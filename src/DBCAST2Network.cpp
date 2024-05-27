#include <iterator>
#include <regex>
#include <fstream>
#include <variant>
#include <sstream>
#include <unordered_map>
#include <cassert>

#include <boost/variant.hpp>

#include "dbcppp/Network.h"
#include "dbcppp/CApi.h"

#include "DBCX3.h"

using namespace dbcppp;
using namespace dbcppp::DBCX3::AST;

namespace
{
using AttributeList = std::vector<variant_attribute_t const*>;
using Description = boost::variant<G_ValueDescriptionSignal, G_ValueDescriptionEnvVar>;

struct SignalCache
{
    AttributeList Attributes;
    Description const* Description_ = nullptr;
    variant_comment_t const* Comment = nullptr;
};

struct MessageCache
{
    std::unordered_map<std::string_view, SignalCache> Signals;
    AttributeList Attributes;
    variant_comment_t const* Comment = nullptr;
};

struct EnvVarCache
{
    AttributeList Attributes;
    Description const* Description_;
    variant_comment_t const* Comment = nullptr;
};

struct NodeCache
{
    AttributeList Attributes;
    variant_comment_t const* Comment = nullptr;
};

struct Cache
{
    AttributeList NetworkAttributes;
    variant_comment_t const* NetworkComment = nullptr;
    std::unordered_map<std::string_view, EnvVarCache> EnvVars;
    std::unordered_map<std::string_view, NodeCache> Nodes;
    std::unordered_map<uint64_t, MessageCache> Messages;
};

} // anon

static auto getVersion(const G_Network& gnet)
{
    return gnet.version.version;
}
static auto getNewSymbols(const G_Network& gnet)
{
    std::vector<std::string> nodes;
    for (const auto& ns : gnet.new_symbols)
    {
        nodes.push_back(ns);
    }
    return nodes;
}
static auto getSignalType(const G_Network& gnet, const G_ValueTable& vt)
{
    std::optional<std::unique_ptr<ISignalType>> signal_type;
    auto iter = std::find_if(gnet.signal_types.begin(), gnet.signal_types.end(),
        [&](const auto& st)
        {
            return st.value_table_name == vt.name;
        });
    if (iter != gnet.signal_types.end())
    {
        auto& st = *iter;
        signal_type = ISignalType::Create(
              std::string(st.name)
            , st.size
            , st.byte_order == '0' ? ISignal::EByteOrder::BigEndian : ISignal::EByteOrder::LittleEndian
            , st.value_type == '+' ? ISignal::EValueType::Unsigned : ISignal::EValueType::Signed
            , st.factor
            , st.offset
            , st.minimum
            , st.maximum
            , std::string(st.unit)
            , st.default_value
            , std::string(st.value_table_name));
    }
    return signal_type;
}
static auto getValueTables(const G_Network& gnet)
{
    std::vector<std::unique_ptr<IValueTable>> value_tables;
    for (const auto& vt : gnet.value_tables)
    {
        auto sig_type = getSignalType(gnet, vt);
        std::vector<std::unique_ptr<IValueEncodingDescription>> copy_ved;
        for (const auto& ved : vt.value_encoding_descriptions)
        {
            auto desc = ved.description;
            auto pved = IValueEncodingDescription::Create(ved.value, std::move(desc));
            copy_ved.push_back(std::move(pved));
        }
        auto nvt = IValueTable::Create(std::string(vt.name), std::move(sig_type), std::move(copy_ved));
        value_tables.push_back(std::move(nvt));
    }
    return value_tables;
}
static auto getBitTiming(const G_Network& gnet)
{
    std::unique_ptr<IBitTiming> bit_timing;
    if (gnet.bit_timing)
    {
        bit_timing = IBitTiming::Create(gnet.bit_timing->baudrate, gnet.bit_timing->BTR1, gnet.bit_timing->BTR2);
    }
    else
    {
        bit_timing = IBitTiming::Create(0, 0, 0);
    }
    return bit_timing;
}

template <class Variant>
class Visitor
    : public boost::static_visitor<void>
{
public:
    Visitor(Variant& var)
        : _var(var)
    {}
    template <class T>
    void operator()(const T& v)
    {
        _var = v;
    }

private:
    Variant& _var;
};

template <class... Args>
auto boost_variant_to_std_variant(const boost::variant<Args...>& old)
{
    using var_t = std::variant<Args...>;
    var_t new_;
    Visitor<var_t> visitor(new_);
    old.apply_visitor(visitor);
    return new_;
}

inline auto boost_variant_to_std_variant(variant_attr_value_t const& attr)
{
    IAttribute::value_t value;

    switch (attr.which()) {
    case 0: {
        IAttribute::value_t v(std::in_place_type<int64_t>, boost::get<int64_t>(attr));
        value.swap(v);
    } break;
    case 1: {
        IAttribute::value_t v(std::in_place_type<double>, boost::get<double>(attr));
        value.swap(v);
    } break;
    case 2: {
        IAttribute::value_t v(std::in_place_type<std::string>, boost::get<std::string>(attr));
        value.swap(v);
    } break;
    default:
        assert(false && "Unhandled variant member");
        break;
    }

    return value;
}

static auto getAttributeValues(const G_Network& gnet, const G_Node& n, Cache const& cache)
{
    std::vector<std::unique_ptr<IAttribute>> attribute_values;

    auto node_it = cache.Nodes.find(n.name);

    if (node_it != cache.Nodes.end()) {
        attribute_values.reserve(node_it->second.Attributes.size());

        for (auto av : node_it->second.Attributes) {
            auto const& attr = boost::get<G_AttributeNode>(*av);
            auto name = attr.attribute_name;
            auto value{boost_variant_to_std_variant(attr.value)};
            auto attribute = IAttribute::Create(std::move(name), IAttributeDefinition::EObjectType::Node, std::move(value));
            attribute_values.emplace_back(std::move(attribute));
        }
    }

    return attribute_values;
}
static auto getComment(const G_Network& gnet, const G_Node& n, Cache const& cache)
{
    std::string comment;
    auto node_it = cache.Nodes.find(n.name);

    if (node_it != cache.Nodes.end()) {
        if (node_it->second.Comment) {
            comment = boost::get<G_CommentNode>(*node_it->second.Comment).comment;
        }
    }

    return comment;
}
static auto getNodes(const G_Network& gnet, Cache const& cache)
{
    std::vector<std::unique_ptr<INode>> nodes;
    for (const auto& n : gnet.nodes)
    {
        auto comment = getComment(gnet, n, cache);
        auto attribute_values = getAttributeValues(gnet, n, cache);
        auto nn = INode::Create(std::string(n.name), std::move(comment), std::move(attribute_values));
        nodes.push_back(std::move(nn));
    }
    return nodes;
}
static auto getAttributeValues(const G_Network& gnet, const G_Message& m, const G_Signal& s, Cache const& cache)
{
    std::vector<std::unique_ptr<IAttribute>> attribute_values;
    auto const message_it = cache.Messages.find(m.id);

    if (message_it != cache.Messages.end()) {
        auto const signal_it = message_it->second.Signals.find(s.name);

        if (signal_it != message_it->second.Signals.end()) {
            attribute_values.reserve(signal_it->second.Attributes.size());

            for (auto av : signal_it->second.Attributes)
            {
                auto const& attr = boost::get<G_AttributeSignal>(*av);
                auto value{boost_variant_to_std_variant(attr.value)};
                auto attribute = IAttribute::Create(std::string(attr.attribute_name), IAttributeDefinition::EObjectType::Signal, std::move(value));
                attribute_values.emplace_back(std::move(attribute));
            }
        }

    }

    return attribute_values;
}
static auto getValueDescriptions(const G_Network& gnet, const G_Message& m, const G_Signal& s, Cache const& cache)
{
    std::vector<std::unique_ptr<IValueEncodingDescription>> value_descriptions;
    auto const message_it = cache.Messages.find(m.id);

    if (message_it != cache.Messages.end()) {
        auto const signal_it = message_it->second.Signals.find(s.name);

        if (signal_it != message_it->second.Signals.end()) {
            if (signal_it->second.Description_) {
                auto const& vds = boost::get<G_ValueDescriptionSignal>(*signal_it->second.Description_).value_descriptions;

                value_descriptions.reserve(vds.size());

                for (const auto& vd : vds)
                {
                    auto desc = vd.description;
                    auto pvd = IValueEncodingDescription::Create(vd.value, std::move(desc));
                    value_descriptions.push_back(std::move(pvd));
                }
            }
        }
    }
    return value_descriptions;
}
static auto getComment(const G_Network& gnet, const G_Message& m, const G_Signal& s, Cache const& cache)
{
    std::string comment;
    auto message_it = cache.Messages.find(m.id);

    if (message_it != cache.Messages.end()) {
        auto signal_it = message_it->second.Signals.find(s.name);

        if (signal_it != message_it->second.Signals.end()) {
            if (signal_it->second.Comment) {
                comment = boost::get<G_CommentSignal>(*signal_it->second.Comment).comment;
            }
        }

    }
    return comment;
}
static auto getSignalExtendedValueType(const G_Network& gnet, const G_Message& m, const G_Signal& s)
{
    ISignal::EExtendedValueType extended_value_type = ISignal::EExtendedValueType::Integer;
    auto iter = std::find_if(gnet.signal_extended_value_types.begin(), gnet.signal_extended_value_types.end(),
        [&](const G_SignalExtendedValueType& sev)
        {
            return sev.message_id == m.id && sev.signal_name == s.name;
        });
    if (iter != gnet.signal_extended_value_types.end())
    {
        switch (iter->value)
        {
        case 1: extended_value_type = ISignal::EExtendedValueType::Float; break;
        case 2: extended_value_type = ISignal::EExtendedValueType::Double; break;
        }
    }
    return extended_value_type;
}
static auto getSignalMultiplexerValues(const G_Network& gnet, const std::string& s, const uint64_t m)
{
    std::vector<std::unique_ptr<ISignalMultiplexerValue>> signal_multiplexer_values;
    for (const auto& gsmv : gnet.signal_multiplexer_values)
    {
        if (gsmv.signal_name == s && gsmv.message_id == m)
        {
            auto switch_name = gsmv.switch_name;
            std::vector<ISignalMultiplexerValue::Range> value_ranges;
            for (const auto& r : gsmv.value_ranges)
            {
                value_ranges.push_back({r.from, r.to});
            }
            auto signal_multiplexer_value = ISignalMultiplexerValue::Create(
                  std::move(switch_name)
                , std::move(value_ranges));
            signal_multiplexer_values.push_back(std::move(signal_multiplexer_value));
        }
    }
    return signal_multiplexer_values;
}
static auto getSignals(const G_Network& gnet, const G_Message& m, Cache const& cache)
{
    std::vector<std::unique_ptr<ISignal>> signals;

    signals.reserve(m.signals.size());

    for (const G_Signal& s : m.signals)
    {
        std::vector<std::string> receivers;
        auto attribute_values = getAttributeValues(gnet, m, s, cache);
        auto value_descriptions = getValueDescriptions(gnet, m, s, cache);
        auto extended_value_type = getSignalExtendedValueType(gnet, m, s);
        auto multiplexer_indicator = ISignal::EMultiplexer::NoMux;
        auto comment = getComment(gnet, m, s, cache);
        auto signal_multiplexer_values = getSignalMultiplexerValues(gnet, s.name, m.id);
        uint64_t multiplexer_switch_value = 0;
        if (s.multiplexer_indicator)
        {
            auto m = *s.multiplexer_indicator;
            if (m.substr(0, 1) == "M")
            {
                multiplexer_indicator = ISignal::EMultiplexer::MuxSwitch;
            }
            else
            {
                multiplexer_indicator = ISignal::EMultiplexer::MuxValue;
                std::string value = m.substr(1, m.size());
                multiplexer_switch_value = std::atoi(value.c_str());
            }
        }

        receivers.reserve(s.receivers.size());

        for (const auto& n : s.receivers)
        {
            receivers.emplace_back(n);
        }

        auto ns = ISignal::Create(
              m.size
            , std::string(s.name)
            , multiplexer_indicator
            , multiplexer_switch_value
            , s.start_bit
            , s.signal_size
            , s.byte_order == '0' ? ISignal::EByteOrder::BigEndian : ISignal::EByteOrder::LittleEndian
            , s.value_type == '+' ? ISignal::EValueType::Unsigned : ISignal::EValueType::Signed
            , s.factor
            , s.offset
            , s.minimum
            , s.maximum
            , std::string(s.unit)
            , std::move(receivers)
            , std::move(attribute_values)
            , std::move(value_descriptions)
            , std::move(comment)
            , extended_value_type
            , std::move(signal_multiplexer_values));
        if (ns->Error(ISignal::EErrorCode::SignalExceedsMessageSize))
        {
            std::cout << "Warning: The signals '" << m.name << "::" << s.name << "'"
                << " start_bit + bit_size exceeds the byte size of the message! Ignoring this error will lead to garbage data when using the decode function of this signal." << std::endl;
        }
        if (ns->Error(ISignal::EErrorCode::WrongBitSizeForExtendedDataType))
        {
            std::cout << "Warning: The signals '" << m.name << "::" << s.name << "'"
                << " bit_size does not fit the bit size of the specified ExtendedValueType." << std::endl;
        }
        if (ns->Error(ISignal::EErrorCode::MaschinesFloatEncodingNotSupported))
        {
            std::cout << "Warning: Signal '" << m.name << "::" << s.name << "'"
                << " This warning appears when a signal uses type float but the system this programm is running on does not uses IEEE 754 encoding for floats." << std::endl;
        }
        if (ns->Error(ISignal::EErrorCode::MaschinesDoubleEncodingNotSupported))
        {
            std::cout << "Warning: Signal '" << m.name << "::" << s.name << "'"
                << " This warning appears when a signal uses type double but the system this programm is running on does not uses IEEE 754 encoding for doubles." << std::endl;
        }
        signals.emplace_back(std::move(ns));
    }
    return signals;
}
static auto getMessageTransmitters(const G_Network& gnet, const G_Message& m)
{
    std::vector<std::string> message_transmitters;
    auto iter_mt = std::find_if(gnet.message_transmitters.begin(), gnet.message_transmitters.end(),
        [&](const G_MessageTransmitter& mt)
        {
            return mt.id == m.id;
        });
    if (iter_mt != gnet.message_transmitters.end())
    {
        for (const auto& t : iter_mt->transmitters)
        {
            message_transmitters.push_back(t);
        }
    }
    return message_transmitters;
}
static auto getAttributeValues(const G_Network& gnet, const G_Message& m, Cache const& cache)
{
    std::vector<std::unique_ptr<IAttribute>> attribute_values;

    auto message_it = cache.Messages.find(m.id);

    if (message_it != cache.Messages.end()) {
        attribute_values.reserve(message_it->second.Attributes.size());

        for (auto av: message_it->second.Attributes) {
            auto const& attr = boost::get<G_AttributeMessage>(*av);
            auto value{boost_variant_to_std_variant(attr.value)};
            auto attribute = IAttribute::Create(std::string(attr.attribute_name), IAttributeDefinition::EObjectType::Message, std::move(value));
            attribute_values.emplace_back(std::move(attribute));
        }
    }
    return attribute_values;
}
static auto getComment(const G_Network& gnet, const G_Message& m, Cache const& cache)
{
    std::string comment;
    auto message_it = cache.Messages.find(m.id);

    if (message_it != cache.Messages.end()) {
        if (message_it->second.Comment) {
            comment = boost::get<G_CommentMessage>(*message_it->second.Comment).comment;
        }
    }
    return comment;
}
static auto getSignalGroups(const G_Network& gnet, const G_Message& m)
{
    std::vector<std::unique_ptr<ISignalGroup>> signal_groups;
    for (const auto& sg : gnet.signal_groups)
    {
        if (sg.message_id == m.id)
        {
            auto name = sg.signal_group_name;
            auto signal_names = sg.signal_names;
            auto signal_group = ISignalGroup::Create(
                  sg.message_id
                , std::move(name)
                , sg.repetitions
                , std::move(signal_names));
            signal_groups.push_back(std::move(signal_group));
        }
    }
    return signal_groups;
}
static auto getMessages(const G_Network& gnet, Cache const& cache)
{
    std::vector<std::unique_ptr<IMessage>> messages;

    messages.reserve(cache.Messages.size());

    for (const auto& m : gnet.messages)
    {
        auto message_transmitters = getMessageTransmitters(gnet, m);
        auto signals = getSignals(gnet, m, cache);
        auto attribute_values = getAttributeValues(gnet, m, cache);
        auto comment = getComment(gnet, m, cache);
        auto signal_groups = getSignalGroups(gnet, m);
        auto msg = IMessage::Create(
              m.id
            , std::string(m.name)
            , m.size
            , std::string(m.transmitter)
            , std::move(message_transmitters)
            , std::move(signals)
            , std::move(attribute_values)
            , std::move(comment)
            , std::move(signal_groups));
        if (msg->Error() == IMessage::EErrorCode::MuxValeWithoutMuxSignal)
        {
            std::cout << "Warning: Message " << msg->Name() << " does have mux value but no mux signal!" << std::endl;
        }
        messages.emplace_back(std::move(msg));
    }
    return messages;
}
static auto getValueDescriptions(const G_Network& gnet, const G_EnvironmentVariable& ev, Cache const& cache)
{
    std::vector<std::unique_ptr<IValueEncodingDescription>> value_descriptions;
    auto env_it = cache.EnvVars.find(ev.name);

    if (env_it != cache.EnvVars.end()) {
        if (env_it->second.Description_) {
            auto const& vds = boost::get<G_ValueDescriptionEnvVar>(*env_it->second.Description_).value_descriptions;

            value_descriptions.reserve(vds.size());

            for (const auto& vd : vds)
            {
                auto desc = vd.description;
                auto pvd = IValueEncodingDescription::Create(vd.value, std::move(desc));
                value_descriptions.push_back(std::move(pvd));
            }
        }
    }
    return value_descriptions;
}
static auto getAttributeValues(const G_Network& gnet, const G_EnvironmentVariable& ev, const Cache& cache)
{
    std::vector<std::unique_ptr<IAttribute>> attribute_values;

    auto env_it = cache.EnvVars.find(ev.name);

    if (env_it != cache.EnvVars.end()) {
        attribute_values.reserve(env_it->second.Attributes.size());

        for (auto av : env_it->second.Attributes) {
            auto const& attr = boost::get<G_AttributeEnvVar>(*av);
            auto value = boost_variant_to_std_variant(attr.value);
            auto attribute = IAttribute::Create(std::string(attr.attribute_name), IAttributeDefinition::EObjectType::EnvironmentVariable, std::move(value));
            attribute_values.push_back(std::move(attribute));
        }
    }

    return attribute_values;
}
static auto getComment(const G_Network& gnet, const G_EnvironmentVariable& ev, Cache const& cache)
{
    std::string comment;
    auto envvar_it = cache.EnvVars.find(ev.name);

    if (envvar_it != cache.EnvVars.end()) {
        if (envvar_it->second.Comment) {
            comment = boost::get<G_CommentEnvVar>(*envvar_it->second.Comment).comment;
        }
    }
    return comment;
}
static auto getEnvironmentVariables(const G_Network& gnet, Cache const& cache)
{
    std::vector<std::unique_ptr<IEnvironmentVariable>> environment_variables;
    for (const auto& ev : gnet.environment_variables)
    {
        IEnvironmentVariable::EVarType var_type;
        IEnvironmentVariable::EAccessType access_type;
        std::vector<std::string> access_nodes = ev.access_nodes;
        auto value_descriptions = getValueDescriptions(gnet, ev, cache);
        auto attribute_values = getAttributeValues(gnet, ev, cache);
        auto comment = getComment(gnet, ev, cache);
        uint64_t data_size = 0;
        switch (ev.var_type)
        {
        case 0: var_type = IEnvironmentVariable::EVarType::Integer; break;
        case 1: var_type = IEnvironmentVariable::EVarType::Float; break;
        case 2: var_type = IEnvironmentVariable::EVarType::String; break;
        }
        access_type = IEnvironmentVariable::EAccessType::Unrestricted;
        if (ev.access_type == "DUMMY_NODE_VECTOR0")         access_type = IEnvironmentVariable::EAccessType::Unrestricted;
        else if (ev.access_type == "DUMMY_NODE_VECTOR1")    access_type = IEnvironmentVariable::EAccessType::Read;
        else if (ev.access_type == "DUMMY_NODE_VECTOR2")    access_type = IEnvironmentVariable::EAccessType::Write;
        else if (ev.access_type == "DUMMY_NODE_VECTOR3")    access_type = IEnvironmentVariable::EAccessType::ReadWrite;
        else if (ev.access_type == "DUMMY_NODE_VECTOR8000") access_type = IEnvironmentVariable::EAccessType::Unrestricted_;
        else if (ev.access_type == "DUMMY_NODE_VECTOR8001") access_type = IEnvironmentVariable::EAccessType::Read_;
        else if (ev.access_type == "DUMMY_NODE_VECTOR8002") access_type = IEnvironmentVariable::EAccessType::Write_;
        else if (ev.access_type == "DUMMY_NODE_VECTOR8003") access_type = IEnvironmentVariable::EAccessType::ReadWrite_;
        for (auto& evd : gnet.environment_variable_datas)
        {
            if (evd.name == ev.name)
            {
                var_type = IEnvironmentVariable::EVarType::Data;
                data_size = evd.size;
                break;
            }
        }
        auto env_var = IEnvironmentVariable::Create(
              std::string(ev.name)
            , var_type
            , ev.minimum
            , ev.maximum
            , std::string(ev.unit)
            , ev.initial_value
            , ev.id
            , access_type
            , std::move(access_nodes)
            , std::move(value_descriptions)
            , data_size
            , std::move(attribute_values)
            , std::move(comment));
        environment_variables.push_back(std::move(env_var));
    }
    return environment_variables;
}
static auto getAttributeDefinitions(const G_Network& gnet)
{
    std::vector<std::unique_ptr<IAttributeDefinition>> attribute_definitions;
    struct VisitorValueType
    {
        IAttributeDefinition::value_type_t operator()(const G_AttributeValueTypeInt& cn)
        {
            IAttributeDefinition::ValueTypeInt vt;
            vt.minimum = cn.minimum;
            vt.maximum = cn.maximum;
            return vt;
        }
        IAttributeDefinition::value_type_t operator()(const G_AttributeValueTypeHex& cn)
        {
            IAttributeDefinition::ValueTypeHex vt;
            vt.minimum = cn.minimum;
            vt.maximum = cn.maximum;
            return vt;
        }
        IAttributeDefinition::value_type_t operator()(const G_AttributeValueTypeFloat& cn)
        {
            IAttributeDefinition::ValueTypeFloat vt;
            vt.minimum = cn.minimum;
            vt.maximum = cn.maximum;
            return vt;
        }
        IAttributeDefinition::value_type_t operator()(const G_AttributeValueTypeString& cn)
        {
            return IAttributeDefinition::ValueTypeString();
        }
        IAttributeDefinition::value_type_t operator()(const G_AttributeValueTypeEnum& cn)
        {
            IAttributeDefinition::ValueTypeEnum vt;
            for (auto& e : cn.values)
            {
                vt.values.emplace_back(e);
            }
            return vt;
        }
    };

    for (const auto& ad : gnet.attribute_definitions)
    {
        IAttributeDefinition::EObjectType object_type;
        auto cvt = ad.value_type;
        if (!ad.object_type)
        {
            object_type = IAttributeDefinition::EObjectType::Network;
        }
        else if (*ad.object_type == "BU_")
        {
            object_type = IAttributeDefinition::EObjectType::Node;
        }
        else if (*ad.object_type == "BO_")
        {
            object_type = IAttributeDefinition::EObjectType::Message;
        }
        else if (*ad.object_type == "SG_")
        {
            object_type = IAttributeDefinition::EObjectType::Signal;
        }
        else
        {
            object_type = IAttributeDefinition::EObjectType::EnvironmentVariable;
        }
        VisitorValueType vvt;
        auto value = boost_variant_to_std_variant(cvt.value);
        std::visit(vvt, value);
        auto nad = IAttributeDefinition::Create(std::move(std::string(ad.name)), object_type, std::visit(vvt, value));
        attribute_definitions.push_back(std::move(nad));
    }
    return attribute_definitions;
}
static auto getAttributeDefaults(const G_Network& gnet)
{
    std::vector<std::unique_ptr<IAttribute>> attribute_defaults;
    for (auto& ad : gnet.attribute_defaults)
    {
        auto value = boost_variant_to_std_variant(ad.value);
        auto nad = IAttribute::Create(std::string(ad.name), IAttributeDefinition::EObjectType::Network, value);
        attribute_defaults.push_back(std::move(nad));
    }
    return attribute_defaults;
}
static auto getAttributeValues(const G_Network& gnet, Cache const& cache)
{
    std::vector<std::unique_ptr<IAttribute>> attribute_values;

    attribute_values.reserve(cache.NetworkAttributes.size());

    for (auto av : cache.NetworkAttributes)
    {
        auto const& attr = boost::get<G_AttributeNetwork>(*av);
        auto value{boost_variant_to_std_variant(attr.value)};
        auto attribute = IAttribute::Create(
            std::string(attr.attribute_name)
            , IAttributeDefinition::EObjectType::Network
            , std::move(value));
        attribute_values.emplace_back(std::move(attribute));
    }
    return attribute_values;
}
static auto getComment(const G_Network& gnet, Cache const& cache)
{
    std::string comment;
    if (cache.NetworkComment) {
        comment = boost::get<G_CommentNetwork>(*cache.NetworkComment).comment;
    }
    return comment;
}

std::unique_ptr<INetwork> DBCAST2Network(const G_Network& gnet)
{
    Cache cache;

    for (const auto& av : gnet.attribute_values)
    {
        switch (av.which()) {
        case 0: {
            auto const& attr = boost::get<G_AttributeNetwork>(av);
            cache.NetworkAttributes.emplace_back(&av);
        } break;
        case 1: {
            auto const& attr = boost::get<G_AttributeNode>(av);
            auto node_it = cache.Nodes.find(attr.node_name);
            if (node_it == cache.Nodes.end()) {
                node_it = cache.Nodes.emplace(attr.node_name, NodeCache()).first;
            }
            node_it->second.Attributes.emplace_back(&av);
        } break;
        case 2: {
            auto const& attr = boost::get<G_AttributeMessage>(av);
            auto it = cache.Messages.find(attr.message_id);
            if (it == cache.Messages.end()) {
                it = cache.Messages.emplace(attr.message_id, MessageCache()).first;
            }

            it->second.Attributes.emplace_back(&av);
        } break;
        case 3: {
            auto const& attr = boost::get<G_AttributeSignal>(av);
            auto message_it = cache.Messages.find(attr.message_id);
            if (message_it == cache.Messages.end()) {
                message_it = cache.Messages.emplace(attr.message_id, MessageCache()).first;
            }

            auto& signals = message_it->second.Signals;
            auto signal_it = signals.find(attr.signal_name);
            if (signal_it == signals.end()) {
                signal_it = signals.emplace(attr.signal_name, SignalCache()).first;
            }

            signal_it->second.Attributes.emplace_back(&av);
        } break;
        case 4: {
            auto const& attr = boost::get<G_AttributeEnvVar>(av);
            auto envvar_it = cache.EnvVars.find(attr.env_var_name);
            if (envvar_it == cache.EnvVars.end()) {
                envvar_it = cache.EnvVars.emplace(attr.env_var_name, EnvVarCache()).first;
            }
            envvar_it->second.Attributes.emplace_back(&av);
        } break;
        default:
            assert(false && "Unhandled variant member");
            break;
        }
    }

    for (const auto& vd : gnet.value_descriptions_sig_env_var)
    {
        switch (vd.description.which()) {
        case 0: {
            auto const& desc = boost::get<G_ValueDescriptionSignal>(vd.description);
            auto message_it = cache.Messages.find(desc.message_id);
            if (message_it == cache.Messages.end()) {
                message_it = cache.Messages.emplace(desc.message_id, MessageCache()).first;
            }

            auto& signals = message_it->second.Signals;
            auto signal_it = signals.find(desc.signal_name);
            if (signal_it == signals.end()) {
                signal_it = signals.emplace(desc.signal_name, SignalCache()).first;
            }

            signal_it->second.Description_ = &vd.description;
        } break;
        case 1: {
            auto const& desc = boost::get<G_ValueDescriptionEnvVar>(vd.description);
            auto envvar_it = cache.EnvVars.find(desc.env_var_name);
            if (envvar_it == cache.EnvVars.end()) {
                envvar_it = cache.EnvVars.emplace(desc.env_var_name, EnvVarCache()).first;
            }
            envvar_it->second.Description_ = &vd.description;
        } break;
        default:
            assert(false && "Unhandled variant member");
            break;
        }
    }

    for (const auto& comment : gnet.comments)
    {
        switch (comment.comment.which()) {
        case 0: {
            auto const& c = boost::get<G_CommentNetwork>(comment.comment);
            cache.NetworkComment = &comment.comment;
        } break;
        case 1: {
            auto const& c = boost::get<G_CommentNode>(comment.comment);
            auto node_it = cache.Nodes.find(c.node_name);
            if (node_it == cache.Nodes.end()) {
                node_it = cache.Nodes.emplace(c.node_name, NodeCache()).first;
            }
            node_it->second.Comment = &comment.comment;
        } break;
        case 2: {
            auto const& c = boost::get<G_CommentMessage>(comment.comment);
            auto message_it = cache.Messages.find(c.message_id);
            if (message_it == cache.Messages.end()) {
                message_it = cache.Messages.emplace(c.message_id, MessageCache()).first;
            }
            message_it->second.Comment = &comment.comment;
        } break;
        case 3: {
            auto const& c = boost::get<G_CommentSignal>(comment.comment);
            auto message_it = cache.Messages.find(c.message_id);
            if (message_it == cache.Messages.end()) {
                message_it = cache.Messages.emplace(c.message_id, MessageCache()).first;
            }

            auto signal_it = message_it->second.Signals.find(c.signal_name);
            if (signal_it == message_it->second.Signals.end()) {
                signal_it = message_it->second.Signals.emplace(c.signal_name, SignalCache()).first;
            }
            signal_it->second.Comment = &comment.comment;
        } break;
        case 4: {
            auto const& c = boost::get<G_CommentEnvVar>(comment.comment);
            auto envvar_it = cache.EnvVars.find(c.env_var_name);
            if (envvar_it == cache.EnvVars.end()) {
                envvar_it = cache.EnvVars.emplace(c.env_var_name, EnvVarCache()).first;
            }
            envvar_it->second.Comment = &comment.comment;
        } break;
        default:
            assert(false && "Unhandled variant member");
            break;
        }
    }

    return INetwork::Create(
          getVersion(gnet)
        , getNewSymbols(gnet)
        , getBitTiming(gnet)
        , getNodes(gnet, cache)
        , getValueTables(gnet)
        , getMessages(gnet, cache)
        , getEnvironmentVariables(gnet, cache)
        , getAttributeDefinitions(gnet)
        , getAttributeDefaults(gnet)
        , getAttributeValues(gnet, cache)
        , getComment(gnet, cache));
}

std::unique_ptr<INetwork> INetwork::LoadDBCFromIs(std::istream& is)
{
    std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    std::unique_ptr<dbcppp::INetwork> network;
    if (auto gnet = dbcppp::DBCX3::ParseFromMemory(str.c_str(), str.c_str() + str.size()))
    {
        network = DBCAST2Network(*gnet);
    }
    return network;
}
extern "C"
{
    DBCPPP_API const dbcppp_Network* dbcppp_NetworkLoadDBCFromFile(const char* filename)
    {
        std::ifstream is(filename);
        auto net = INetwork::LoadDBCFromIs(is);
        return reinterpret_cast<const dbcppp_Network*>(net.release());
    }
    DBCPPP_API const dbcppp_Network* dbcppp_NetworkLoadDBCFromMemory(const char* data)
    {
        std::istringstream iss(data);
        auto net = INetwork::LoadDBCFromIs(iss);
        return reinterpret_cast<const dbcppp_Network*>(net.release());
    }
}
