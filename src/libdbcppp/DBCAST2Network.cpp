#include <iterator>
#include <regex>
#include <fstream>

#include <boost/hof/apply.hpp>

#include "../../include/dbcppp/Network.h"
#include "../../include/dbcppp/CApi.h"

#include "DBC_Grammar.h"

using namespace dbcppp;

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
    std::optional<std::unique_ptr<SignalType>> result;
    auto iter = std::find_if(gnet.signal_types.begin(), gnet.signal_types.end(),
        [&](const auto& st)
        {
            return st.value_table_name == vt.name;
        });
    if (iter != gnet.signal_types.end())
    {
        auto& st = *iter;
        result = SignalType::create(
              std::string(st.name)
            , st.size
            , st.byte_order == '0' ? Signal::ByteOrder::BigEndian : Signal::ByteOrder::LittleEndian
            , st.value_type == '+' ? Signal::ValueType::Unsigned : Signal::ValueType::Signed
            , st.factor
            , st.offset
            , st.minimum
            , st.maximum
            , std::string(st.unit)
            , st.default_value
            , std::string(st.value_table_name));
    }
    return result;
}
static auto getValueTables(const G_Network& gnet)
{
    std::vector<std::unique_ptr<ValueTable>> result;
    for (const auto& vt : gnet.value_tables)
    {
        auto sig_type = getSignalType(gnet, vt);
        auto copy_ved = vt.value_encoding_descriptions;
        auto nvt = ValueTable::create(std::string(vt.name), std::move(sig_type), std::move(copy_ved));
        result.push_back(std::move(nvt));
    }
    return result;
}
static auto getBitTiming(const G_Network& gnet)
{
    std::unique_ptr<BitTiming> result;
    if (gnet.bit_timing)
    {
        result = BitTiming::create(gnet.bit_timing->baudrate, gnet.bit_timing->BTR1, gnet.bit_timing->BTR2);
    }
    else
    {
        result = BitTiming::create(0, 0, 0);
    }
    return result;
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
    std::vector<std::unique_ptr<Attribute>> result;
    for (const variant_attribute_t& av : gnet.attribute_values)
    {
        auto av_ = boost_variant_to_std_variant(av);
        if (auto pav = std::get_if<G_AttributeNode>(&av_); pav && pav->node_name == n.name)
        {
            auto name = pav->attribute_name;
            auto value = boost_variant_to_std_variant(pav->value);
            auto attribute = Attribute::create(std::move(name), AttributeDefinition::ObjectType::Node, std::move(value));
            result.push_back(std::move(attribute));
        }
    }
    return result;
}
static auto getComment(const G_Network& gnet, const G_Node& n)
{
    std::string result;
    auto iter_comment = std::find_if(gnet.comments.begin(), gnet.comments.end(),
        [&](const variant_comment_t& c)
        {
            auto c_ = boost_variant_to_std_variant(c);
            auto pcn = std::get_if<G_CommentNode>(&c_);
            return pcn && pcn->node_name == n.name;
        });
    if (iter_comment != gnet.comments.end())
    {
        auto comment_ = boost_variant_to_std_variant(*iter_comment);
        result = std::get<G_CommentNode>(comment_).comment;
    }
    return result;
}
static auto getNodes(const G_Network& gnet)
{
    std::vector<std::unique_ptr<Node>> result;
    for (const auto& n : gnet.nodes)
    {
        auto comment = getComment(gnet, n);
        auto attribute_values = getAttributeValues(gnet, n);
        auto nn = Node::create(std::string(n.name), std::move(comment), std::move(attribute_values));
        result.push_back(std::move(nn));
    }
    return result;
}
static auto getAttributeValues(const G_Network& gnet, const G_Message& m, const G_Signal& s)
{
    std::vector<std::unique_ptr<Attribute>> result;
    for (const auto& vav : gnet.attribute_values)
    {
        auto vav_ = boost_variant_to_std_variant(vav);
        if (auto pas = std::get_if<G_AttributeSignal>(&vav_))
        {
            const auto& av = *pas;
            if (av.message_id == m.id && av.signal_name == s.name)
            {
                auto value = boost_variant_to_std_variant(av.value);
                auto attribute = Attribute::create(std::string(av.attribute_name), AttributeDefinition::ObjectType::Signal, std::move(value));
                result.push_back(std::move(attribute));
            }
        }
    }
    return result;
}
static auto getValueDescriptions(const G_Network& gnet, const G_Message& m, const G_Signal& s)
{
    std::vector<std::tuple<int64_t, std::string>> result;
    for (const auto& vds : gnet.value_descriptions)
    {
        auto vds_description_ = boost_variant_to_std_variant(vds.description);
        if (auto pvds = std::get_if<G_ValueDescriptionSignal>(&vds_description_);
            pvds && pvds->message_id == m.id && pvds->signal_name == s.name)
        {
            result = std::get<G_ValueDescriptionSignal>(vds_description_).value_descriptions;
            break;
        }
    }
    return result;
}
static auto getComment(const G_Network& gnet, const G_Message& m, const G_Signal& s)
{
    std::string result;
    for (const auto& c : gnet.comments)
    {
        auto c_ = boost_variant_to_std_variant(c);
        if (auto pcs = std::get_if<G_CommentSignal>(&c_);
            pcs && pcs->message_id == m.id && pcs->signal_name == s.name)
        {
            result = std::get<G_CommentSignal>(c_).comment;
            break;
        }
    }
    return result;
}
static auto getSignalExtendedValueType(const G_Network& gnet, const G_Message& m, const G_Signal& s)
{
    Signal::ExtendedValueType result = Signal::ExtendedValueType::Integer;
    auto iter = std::find_if(gnet.signal_extended_value_types.begin(), gnet.signal_extended_value_types.end(),
        [&](const G_SignalExtendedValueType& sev)
        {
            return sev.message_id == m.id && sev.signal_name == s.name;
        });
    if (iter != gnet.signal_extended_value_types.end())
    {
        switch (iter->value)
        {
        case 1: result = Signal::ExtendedValueType::Float; break;
        case 2: result = Signal::ExtendedValueType::Double; break;
        }
    }
    return result;
}
static auto getSignals(const G_Network& gnet, const G_Message& m)
{
    std::vector<std::unique_ptr<Signal>> result;
    for (const G_Signal& s : m.signals)
    {
        std::vector<std::string> receivers;
        auto attribute_values = getAttributeValues(gnet, m, s);
        auto value_descriptions = getValueDescriptions(gnet, m, s);
        auto extended_value_type = getSignalExtendedValueType(gnet, m, s);
        auto multiplexer_indicator = Signal::Multiplexer::NoMux;
        auto comment = getComment(gnet, m, s);
        uint64_t multiplexer_switch_value = 0;
        if (s.multiplexer_indicator)
        {
            auto m = *s.multiplexer_indicator;
            if (m.substr(0, 1) == "M")
            {
                multiplexer_indicator = Signal::Multiplexer::MuxSwitch;
            }
            else
            {
                multiplexer_indicator = Signal::Multiplexer::MuxValue;
                std::string value = m.substr(1, m.size());
                multiplexer_switch_value = std::atoi(value.c_str());
            }
        }
        for (const auto& n : s.receivers)
        {
            receivers.push_back(n);
        }
        auto ns = Signal::create(
              m.size
            , std::string(s.name)
            , multiplexer_indicator
            , multiplexer_switch_value
            , s.start_bit
            , s.signal_size
            , s.byte_order == '0' ? Signal::ByteOrder::BigEndian : Signal::ByteOrder::LittleEndian
            , s.value_type == '+' ? Signal::ValueType::Unsigned : Signal::ValueType::Signed
            , s.factor
            , s.offset
            , s.minimum
            , s.maximum
            , std::string(s.unit)
            , std::move(receivers)
            , std::move(attribute_values)
            , std::move(value_descriptions)
            , std::move(comment)
            , extended_value_type);
        if (ns->getError(Signal::ErrorCode::SignalExceedsMessageSize))
        {
            std::cout << "Warning: The signals '" << m.name << "::" << s.name << "'"
                << " start_bit + bit_size exceeds the byte size of the message! Ignoring this error will lead to garbage data when using the decode function of this signal." << std::endl;
        }
        if (ns->getError(Signal::ErrorCode::WrongBitSizeForExtendedDataType))
        {
            std::cout << "Warning: The signals '" << m.name << "::" << s.name << "'"
                << " bit_size does not fit the bit size of the specified ExtendedValueType." << std::endl;
        }
        if (ns->getError(Signal::ErrorCode::MaschinesFloatEncodingNotSupported))
        {
            std::cout << "Warning: Signal '" << m.name << "::" << s.name << "'"
                << " This warning appears when a signal uses type float but the system this programm is running on does not uses IEEE 754 encoding for floats." << std::endl;
        }
        if (ns->getError(Signal::ErrorCode::MaschinesDoubleEncodingNotSupported))
        {
            std::cout << "Warning: Signal '" << m.name << "::" << s.name << "'"
                << " This warning appears when a signal uses type double but the system this programm is running on does not uses IEEE 754 encoding for doubles." << std::endl;
        }
        result.push_back(std::move(ns));
    }
    return result;
}
static auto getMessageTransmitters(const G_Network& gnet, const G_Message& m)
{
    std::vector<std::string> result;
    auto iter_mt = std::find_if(gnet.message_transmitters.begin(), gnet.message_transmitters.end(),
        [&](const G_MessageTransmitter& mt)
        {
            return mt.id == m.id;
        });
    if (iter_mt != gnet.message_transmitters.end())
    {
        for (const auto& t : iter_mt->transmitters)
        {
            result.push_back(t);
        }
    }
    return result;
}
static auto getAttributeValues(const G_Network& gnet, const G_Message& m)
{
    std::vector<std::unique_ptr<Attribute>> result;
    for (const auto& vav : gnet.attribute_values)
    {
        auto vav_ = boost_variant_to_std_variant(vav);
        if (auto pam = std::get_if<G_AttributeMessage>(&vav_))
        {
            const auto& av = *pam;
            if (av.message_id == m.id)
            {
                auto value = boost_variant_to_std_variant(av.value);
                auto attribute = Attribute::create(std::string(av.attribute_name), AttributeDefinition::ObjectType::Message, std::move(value));
                result.push_back(std::move(attribute));
            }
        }
    }
    return result;
}
static auto getComment(const G_Network& gnet, const G_Message& m)
{
    std::string result;
    for (const auto& c : gnet.comments)
    {
        auto c_ = boost_variant_to_std_variant(c);
        if (auto pcm = std::get_if<G_CommentMessage>(&c_);
            pcm && pcm->message_id == m.id)
        {
            result = std::get<G_CommentMessage>(c_).comment;
            break;
        }
    }
    return result;
}
static auto getMessages(const G_Network& gnet)
{
    std::vector<std::unique_ptr<Message>> result;
    for (const auto& m : gnet.messages)
    {
        auto message_transmitters = getMessageTransmitters(gnet, m);
        auto signals = getSignals(gnet, m);
        auto attribute_values = getAttributeValues(gnet, m);
        auto comment = getComment(gnet, m);
        auto msg = Message::create(
              m.id
            , std::string(m.name)
            , m.size
            , std::string(m.transmitter)
            , std::move(message_transmitters)
            , std::move(signals)
            , std::move(attribute_values)
            , std::move(comment));
        if (msg->getError() == Message::ErrorCode::MuxValeWithoutMuxSignal)
        {
            std::cout << "Warning: Message " << msg->getName() << " does have mux value but no mux signal!" << std::endl;
        }
        result.push_back(std::move(msg));
    }
    return result;
}
static auto getValueDescriptions(const G_Network& gnet, const G_EnvironmentVariable& ev)
{
    std::vector<std::tuple<int64_t, std::string>> value_descriptions;
    for (const auto& vds : gnet.value_descriptions)
    {
        auto vds_description = boost_variant_to_std_variant(vds.description);
        if (auto pvde = std::get_if<G_ValueDescriptionEnvVar>(&vds_description);
            pvde && pvde->env_var_name == ev.name)
        {
            value_descriptions = std::get<G_ValueDescriptionEnvVar>(vds_description).value_descriptions;
            break;
        }
    }
    return value_descriptions;
}
static auto getAttributeValues(const G_Network& gnet, const G_EnvironmentVariable& ev)
{
    std::vector<std::unique_ptr<Attribute>> result;
    for (const auto& vav : gnet.attribute_values)
    {
        auto vav_ = boost_variant_to_std_variant(vav);
        if (auto paev = std::get_if<G_AttributeEnvVar>(&vav_))
        {
            const auto& av = *paev;
            if (av.env_var_name == ev.name)
            {
                auto value = boost_variant_to_std_variant(av.value);
                auto attribute = Attribute::create(std::string(av.attribute_name), AttributeDefinition::ObjectType::EnvironmentVariable, std::move(value));
                result.push_back(std::move(attribute));
            }
        }
    }
    return result;
}
static auto getComment(const G_Network& gnet, const G_EnvironmentVariable& ev)
{
    std::string result;
    for (const auto& c : gnet.comments)
    {
        auto c_ = boost_variant_to_std_variant(c);
        if (auto pce = std::get_if<G_CommentEnvVar>(&c_);
            pce && pce->env_var_name == ev.name)
        {
            result = std::get<G_CommentEnvVar>(c_).comment;
            break;
        }
    }
    return result;
}
static auto getEnvironmentVariables(const G_Network& gnet)
{
    std::vector<std::unique_ptr<EnvironmentVariable>> result;
    for (const auto& ev : gnet.environment_variables)
    {
        EnvironmentVariable::VarType var_type;
        EnvironmentVariable::AccessType access_type;
        std::vector<std::string> access_nodes = ev.access_nodes;
        auto value_descriptions = getValueDescriptions(gnet, ev);
        auto attribute_values = getAttributeValues(gnet, ev);
        auto comment = getComment(gnet, ev);
        uint64_t data_size = 0;
        switch (ev.var_type)
        {
        case 0: var_type = EnvironmentVariable::VarType::Integer; break;
        case 1: var_type = EnvironmentVariable::VarType::Float; break;
        case 2: var_type = EnvironmentVariable::VarType::String; break;
        }
        if (ev.access_type == "DUMMY_NODE_VECTOR0")
        {
            access_type = EnvironmentVariable::AccessType::Unrestricted;
        }
        else if (ev.access_type == "DUMMY_NODE_VECTOR1")
        {
            access_type = EnvironmentVariable::AccessType::Read;
        }
        else if (ev.access_type == "DUMMY_NODE_VECTOR2")
        {
            access_type = EnvironmentVariable::AccessType::Write;
        }
        else
        {
            access_type = EnvironmentVariable::AccessType::ReadWrite;
        }
        for (auto& evd : gnet.environment_variable_datas)
        {
            if (evd.name == ev.name)
            {
                var_type = EnvironmentVariable::VarType::Data;
                data_size = evd.size;
                break;
            }
        }
        auto env_var = EnvironmentVariable::create(
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
        result.push_back(std::move(env_var));
    }
    return result;
}
static auto getAttributeDefinitions(const G_Network& gnet)
{
    std::vector<std::unique_ptr<AttributeDefinition>> result;
    struct VisitorValueType
    {
        AttributeDefinition::value_type_t operator()(const G_AttributeValueTypeInt& cn)
        {
            AttributeDefinition::ValueTypeInt vt;
            vt.minimum = cn.minimum;
            vt.maximum = cn.maximum;
            return vt;
        }
        AttributeDefinition::value_type_t operator()(const G_AttributeValueTypeHex& cn)
        {
            AttributeDefinition::ValueTypeHex vt;
            vt.minimum = cn.minimum;
            vt.maximum = cn.maximum;
            return vt;
        }
        AttributeDefinition::value_type_t operator()(const G_AttributeValueTypeFloat& cn)
        {
            AttributeDefinition::ValueTypeFloat vt;
            vt.minimum = cn.minimum;
            vt.maximum = cn.maximum;
            return vt;
        }
        AttributeDefinition::value_type_t operator()(const G_AttributeValueTypeString& cn)
        {
            return AttributeDefinition::ValueTypeString();
        }
        AttributeDefinition::value_type_t operator()(const G_AttributeValueTypeEnum& cn)
        {
            AttributeDefinition::ValueTypeEnum vt;
            for (auto& e : cn.values)
            {
                vt.values.emplace_back(e);
            }
            return vt;
        }
    };
    for (const auto& ad : gnet.attribute_definitions)
    {
        AttributeDefinition::ObjectType object_type;
        auto cvt = ad.value_type;
        if (!ad.object_type)
        {
            object_type = AttributeDefinition::ObjectType::Network;
        }
        else if (*ad.object_type == "BU_")
        {
            object_type = AttributeDefinition::ObjectType::Node;
        }
        else if (*ad.object_type == "BO_")
        {
            object_type = AttributeDefinition::ObjectType::Message;
        }
        else if (*ad.object_type == "SG_")
        {
            object_type = AttributeDefinition::ObjectType::Signal;
        }
        else
        {
            object_type = AttributeDefinition::ObjectType::EnvironmentVariable;
        }
        VisitorValueType vvt;
        auto value = boost_variant_to_std_variant(cvt.value);
        std::visit(vvt, value);
        auto nad = AttributeDefinition::create(std::move(std::string(ad.name)), object_type, std::visit(vvt, value));
        result.push_back(std::move(nad));
    }
    return result;
}
static auto getAttributeDefaults(const G_Network& gnet)
{
    std::vector<std::unique_ptr<Attribute>> result;
    for (auto& ad : gnet.attribute_defaults)
    {
        auto value = boost_variant_to_std_variant(ad.value);
        auto nad = Attribute::create(std::string(ad.name), AttributeDefinition::ObjectType::Network, value);
        result.push_back(std::move(nad));
    }
    return result;
}
static auto getAttributeValues(const G_Network& gnet)
{
    std::vector<std::unique_ptr<Attribute>> result;
    for (const auto& av : gnet.attribute_values)
    {
        auto av_ = boost_variant_to_std_variant(av);
        if (auto pan = std::get_if<G_AttributeNetwork>(&av_))
        {
            auto av_ = *pan;
            auto value = boost_variant_to_std_variant(av_.value);
            auto attribute = Attribute::create(
                std::string(av_.attribute_name)
                , AttributeDefinition::ObjectType::Network
                , std::move(value));
            result.push_back(std::move(attribute));
        }
    }
    return result;
}
static auto getComment(const G_Network& gnet)
{
    std::string result;
    for (const auto& c : gnet.comments)
    {
        auto c_ = boost_variant_to_std_variant(c);
        if (auto pcn = std::get_if<G_CommentNetwork>(&c_))
        {
            result = pcn->comment;
            break;
        }
    }
    return result;
}

std::unique_ptr<Network> DBCAST2Network(const G_Network& gnet)
{
    return Network::create(
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

std::unique_ptr<Network> Network::loadDBCFromIs(std::istream& is)
{
    std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    G_Network gnetwork;
    std::unique_ptr<dbcppp::Network> network;
    if (dbcppp::NetworkGrammar<std::string::iterator>::parse(str.begin(), str.end(), gnetwork))
    {
        network = DBCAST2Network(gnetwork);
    }
    return network;
}
extern "C"
{
    DBCPPP_API const dbcppp_Network* dbcppp_NetworkLoadDBCFromFile(const char* filename)
    {
        std::ifstream is(filename);
        std::unique_ptr<Network> result = Network::loadDBCFromIs(is);
        return reinterpret_cast<const dbcppp_Network*>(result.release());
    }
}