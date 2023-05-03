#include <iterator>
#include <regex>
#include <fstream>
#include <variant>
#include <sstream>

#include <boost/variant.hpp>

#include "dbcppp/Network.h"
#include "dbcppp/CApi.h"

#include "DBCX3.h"

using namespace dbcppp;
using namespace dbcppp::DBCX3::AST;

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

static auto getAttributeValues(const G_Network& gnet, const G_Node& n)
{
    std::vector<std::unique_ptr<IAttribute>> attribute_values;
    for (const variant_attribute_t& av : gnet.attribute_values)
    {
        auto av_ = boost_variant_to_std_variant(av);
        if (auto pav = std::get_if<G_AttributeNode>(&av_); pav && pav->node_name == n.name)
        {
            auto name = pav->attribute_name;
            auto value = boost_variant_to_std_variant(pav->value);
            auto attribute = IAttribute::Create(std::move(name), IAttributeDefinition::EObjectType::Node, std::move(value));
            attribute_values.push_back(std::move(attribute));
        }
    }
    return attribute_values;
}
static auto getComment(const G_Network& gnet, const G_Node& n)
{
    std::string comment;
    auto iter_comment = std::find_if(gnet.comments.begin(), gnet.comments.end(),
        [&](const auto& c)
        {
            auto c_ = boost_variant_to_std_variant(c.comment);
            auto pcn = std::get_if<G_CommentNode>(&c_);
            return pcn && pcn->node_name == n.name;
        });
    if (iter_comment != gnet.comments.end())
    {
        auto comment_ = boost_variant_to_std_variant(iter_comment->comment);
        comment = std::get<G_CommentNode>(comment_).comment;
    }
    return comment;
}
static auto getNodes(const G_Network& gnet)
{
    std::vector<std::unique_ptr<INode>> nodes;
    for (const auto& n : gnet.nodes)
    {
        auto comment = getComment(gnet, n);
        auto attribute_values = getAttributeValues(gnet, n);
        auto nn = INode::Create(std::string(n.name), std::move(comment), std::move(attribute_values));
        nodes.push_back(std::move(nn));
    }
    return nodes;
}
static auto getAttributeValues(const G_Network& gnet, const G_Message& m, const G_Signal& s)
{
    std::vector<std::unique_ptr<IAttribute>> attribute_values;
    for (const auto& vav : gnet.attribute_values)
    {
        auto vav_ = boost_variant_to_std_variant(vav);
        if (auto pas = std::get_if<G_AttributeSignal>(&vav_))
        {
            const auto& av = *pas;
            if (av.message_id == m.id && av.signal_name == s.name)
            {
                auto value = boost_variant_to_std_variant(av.value);
                auto attribute = IAttribute::Create(std::string(av.attribute_name), IAttributeDefinition::EObjectType::Signal, std::move(value));
                attribute_values.push_back(std::move(attribute));
            }
        }
    }
    return attribute_values;
}
static auto getValueDescriptions(const G_Network& gnet, const G_Message& m, const G_Signal& s)
{
    std::vector<std::unique_ptr<IValueEncodingDescription>> value_descriptions;
    for (const auto& vds : gnet.value_descriptions_sig_env_var)
    {
        auto vds_description_ = boost_variant_to_std_variant(vds.description);
        if (auto pvds = std::get_if<G_ValueDescriptionSignal>(&vds_description_);
            pvds && pvds->message_id == m.id && pvds->signal_name == s.name)
        {
            auto vds = std::get<G_ValueDescriptionSignal>(vds_description_).value_descriptions;
            for (const auto& vd : vds)
            {
                auto desc = vd.description;
                auto pvd = IValueEncodingDescription::Create(vd.value, std::move(desc));
                value_descriptions.push_back(std::move(pvd));
            }
            break;
        }
    }
    return value_descriptions;
}
static auto getComment(const G_Network& gnet, const G_Message& m, const G_Signal& s)
{
    std::string comment;
    for (const auto& c : gnet.comments)
    {
        auto c_ = boost_variant_to_std_variant(c.comment);
        if (auto pcs = std::get_if<G_CommentSignal>(&c_);
            pcs && pcs->message_id == m.id && pcs->signal_name == s.name)
        {
            comment = std::get<G_CommentSignal>(c_).comment;
            break;
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
static auto getSignals(const G_Network& gnet, const G_Message& m)
{
    std::vector<std::unique_ptr<ISignal>> signals;
    for (const G_Signal& s : m.signals)
    {
        std::vector<std::string> receivers;
        auto attribute_values = getAttributeValues(gnet, m, s);
        auto value_descriptions = getValueDescriptions(gnet, m, s);
        auto extended_value_type = getSignalExtendedValueType(gnet, m, s);
        auto multiplexer_indicator = ISignal::EMultiplexer::NoMux;
        auto comment = getComment(gnet, m, s);
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
        for (const auto& n : s.receivers)
        {
            receivers.push_back(n);
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
        signals.push_back(std::move(ns));
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
static auto getAttributeValues(const G_Network& gnet, const G_Message& m)
{
    std::vector<std::unique_ptr<IAttribute>> attribute_values;
    for (const auto& vav : gnet.attribute_values)
    {
        auto vav_ = boost_variant_to_std_variant(vav);
        if (auto pam = std::get_if<G_AttributeMessage>(&vav_))
        {
            const auto& av = *pam;
            if (av.message_id == m.id)
            {
                auto value = boost_variant_to_std_variant(av.value);
                auto attribute = IAttribute::Create(std::string(av.attribute_name), IAttributeDefinition::EObjectType::Message, std::move(value));
                attribute_values.push_back(std::move(attribute));
            }
        }
    }
    return attribute_values;
}
static auto getComment(const G_Network& gnet, const G_Message& m)
{
    std::string comment;
    for (const auto& c : gnet.comments)
    {
        auto c_ = boost_variant_to_std_variant(c.comment);
        if (auto pcm = std::get_if<G_CommentMessage>(&c_);
            pcm && pcm->message_id == m.id)
        {
            comment = std::get<G_CommentMessage>(c_).comment;
            break;
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
static auto getMessages(const G_Network& gnet)
{
    std::vector<std::unique_ptr<IMessage>> messages;
    for (const auto& m : gnet.messages)
    {
        auto message_transmitters = getMessageTransmitters(gnet, m);
        auto signals = getSignals(gnet, m);
        auto attribute_values = getAttributeValues(gnet, m);
        auto comment = getComment(gnet, m);
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
        messages.push_back(std::move(msg));
    }
    return messages;
}
static auto getValueDescriptions(const G_Network& gnet, const G_EnvironmentVariable& ev)
{
    std::vector<std::unique_ptr<IValueEncodingDescription>> value_descriptions;
    for (const auto& vds : gnet.value_descriptions_sig_env_var)
    {
        auto vds_description = boost_variant_to_std_variant(vds.description);
        if (auto pvde = std::get_if<G_ValueDescriptionEnvVar>(&vds_description);
            pvde && pvde->env_var_name == ev.name)
        {
            auto vds = std::get<G_ValueDescriptionEnvVar>(vds_description).value_descriptions;
            for (const auto& vd : vds)
            {
                auto desc = vd.description;
                auto pvd = IValueEncodingDescription::Create(vd.value, std::move(desc));
                value_descriptions.push_back(std::move(pvd));
            }
            break;
        }
    }
    return value_descriptions;
}
static auto getAttributeValues(const G_Network& gnet, const G_EnvironmentVariable& ev)
{
    std::vector<std::unique_ptr<IAttribute>> attribute_values;
    for (const auto& vav : gnet.attribute_values)
    {
        auto vav_ = boost_variant_to_std_variant(vav);
        if (auto paev = std::get_if<G_AttributeEnvVar>(&vav_))
        {
            const auto& av = *paev;
            if (av.env_var_name == ev.name)
            {
                auto value = boost_variant_to_std_variant(av.value);
                auto attribute = IAttribute::Create(std::string(av.attribute_name), IAttributeDefinition::EObjectType::EnvironmentVariable, std::move(value));
                attribute_values.push_back(std::move(attribute));
            }
        }
    }
    return attribute_values;
}
static auto getComment(const G_Network& gnet, const G_EnvironmentVariable& ev)
{
    std::string comment;
    for (const auto& c : gnet.comments)
    {
        auto c_ = boost_variant_to_std_variant(c.comment);
        if (auto pce = std::get_if<G_CommentEnvVar>(&c_);
            pce && pce->env_var_name == ev.name)
        {
            comment = std::get<G_CommentEnvVar>(c_).comment;
            break;
        }
    }
    return comment;
}
static auto getEnvironmentVariables(const G_Network& gnet)
{
    std::vector<std::unique_ptr<IEnvironmentVariable>> environment_variables;
    for (const auto& ev : gnet.environment_variables)
    {
        IEnvironmentVariable::EVarType var_type;
        IEnvironmentVariable::EAccessType access_type;
        std::vector<std::string> access_nodes = ev.access_nodes;
        auto value_descriptions = getValueDescriptions(gnet, ev);
        auto attribute_values = getAttributeValues(gnet, ev);
        auto comment = getComment(gnet, ev);
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
static auto getAttributeValues(const G_Network& gnet)
{
    std::vector<std::unique_ptr<IAttribute>> attribute_values;
    for (const auto& av : gnet.attribute_values)
    {
        auto av_ = boost_variant_to_std_variant(av);
        if (auto pan = std::get_if<G_AttributeNetwork>(&av_))
        {
            auto av_ = *pan;
            auto value = boost_variant_to_std_variant(av_.value);
            auto attribute = IAttribute::Create(
                std::string(av_.attribute_name)
                , IAttributeDefinition::EObjectType::Network
                , std::move(value));
            attribute_values.push_back(std::move(attribute));
        }
    }
    return attribute_values;
}
static auto getComment(const G_Network& gnet)
{
    std::string comment;
    for (const auto& c : gnet.comments)
    {
        auto c_ = boost_variant_to_std_variant(c.comment);
        if (auto pcn = std::get_if<G_CommentNetwork>(&c_))
        {
            comment = pcn->comment;
            break;
        }
    }
    return comment;
}

std::unique_ptr<INetwork> DBCAST2Network(const G_Network& gnet)
{
    return INetwork::Create(
          getVersion(gnet)
        , getNewSymbols(gnet)
        , getBitTiming(gnet)
        , getNodes(gnet)
        , getValueTables(gnet)
        , getMessages(gnet)
        , getEnvironmentVariables(gnet)
        , getAttributeDefinitions(gnet)
        , getAttributeDefaults(gnet)
        , getAttributeValues(gnet)
        , getComment(gnet));
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