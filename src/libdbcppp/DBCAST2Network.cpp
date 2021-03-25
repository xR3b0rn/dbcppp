
#include <boost/spirit/home/x3.hpp>

#include <boost/log/trivial.hpp>
#include <iterator>
#include <regex>
#include <fstream>

#include "../../include/dbcppp/Network.h"
#include "../../include/dbcppp/CApi.h"

#include "DBCParser.h"
// #include "DBC_Grammar.h"

using namespace dbcppp;

static auto getVersion(const GNetwork& gnet)
{
    return gnet.version.version;
}
static auto getNewSymbols(const GNetwork& gnet)
{
    std::vector<std::string> nodes;
    for (const auto& ns : gnet.new_symbols)
    {
        nodes.push_back(ns);
    }
    return nodes;
}
static auto getSignalType(const GNetwork& gnet, const GValueTable& vt)
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
static auto getValueTables(const GNetwork& gnet)
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
static auto getBitTiming(const GNetwork& gnet)
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
static auto getAttributeValues(const GNetwork& gnet, const GNode& n)
{
    std::vector<std::unique_ptr<Attribute>> result;
    for (const variant_attribute_t& av : gnet.attribute_values)
    {
        if (auto pav = std::get_if<GAttributeNode>(&av); pav && pav->node_name == n.name)
        {
            auto name = pav->attribute_name;
            auto value = pav->value;
            auto attribute = Attribute::create(std::move(name), AttributeDefinition::ObjectType::Node, std::move(value));
            result.push_back(std::move(attribute));
        }
    }
    return result;
}
static auto getComment(const GNetwork& gnet, const GNode& n)
{
    std::string result;
    auto iter_comment = std::find_if(gnet.comments.begin(), gnet.comments.end(),
        [&](const variant_comment_t& c)
        {
            auto pcn = std::get_if<GCommentNode>(&c);
            return pcn && pcn->node_name == n.name;
        });
    if (iter_comment != gnet.comments.end())
    {
        result = std::get<GCommentNode>(*iter_comment).comment;
    }
    return result;
}
static auto getNodes(const GNetwork& gnet)
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
static auto getAttributeValues(const GNetwork& gnet, const GMessage& m, const GSignal& s)
{
    std::vector<std::unique_ptr<Attribute>> result;
    for (const auto& vav : gnet.attribute_values)
    {
        if (auto pas = std::get_if<GAttributeSignal>(&vav))
        {
            const auto& av = *pas;
            if (av.message_id == m.id && av.signal_name == s.name)
            {
                auto value = av.value;
                auto attribute = Attribute::create(std::string(av.attribute_name), AttributeDefinition::ObjectType::Signal, std::move(value));
                result.push_back(std::move(attribute));
            }
        }
    }
    return result;
}
static auto getValueDescriptions(const GNetwork& gnet, const GMessage& m, const GSignal& s)
{
    std::vector<std::tuple<int64_t, std::string>> result;
    for (const auto& vds : gnet.value_descriptions)
    {
        if (auto pvds = std::get_if<GValueDescriptionSignal>(&vds.description);
            pvds && pvds->message_id == m.id && pvds->signal_name == s.name)
        {
            result = std::get<GValueDescriptionSignal>(vds.description).value_descriptions;
            break;
        }
    }
    return result;
}
static auto getComment(const GNetwork& gnet, const GMessage& m, const GSignal& s)
{
    std::string result;
    for (const auto& c : gnet.comments)
    {
        if (auto pcs = std::get_if<GCommentSignal>(&c);
            pcs && pcs->message_id == m.id && pcs->signal_name == s.name)
        {
            result = std::get<GCommentSignal>(c).comment;
            break;
        }
    }
    return result;
}
static auto getSignalExtendedValueType(const GNetwork& gnet, const GMessage& m, const GSignal& s)
{
    Signal::ExtendedValueType result = Signal::ExtendedValueType::Integer;
    auto iter = std::find_if(gnet.signal_extended_value_types.begin(), gnet.signal_extended_value_types.end(),
        [&](const GSignalExtendedValueType& sev)
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
static auto getSignalMultiplexerValues(const GNetwork& gnet, const GMessage& m, const GSignal& s)
{
    std::vector<std::unique_ptr<SignalMultiplexerValue>> result;
    for (const auto& gsmv : gnet.signal_multiplexer_values)
    {
        if (gsmv.message_id == m.id && gsmv.signal_name == s.name)
        {
            auto message_id = gsmv.message_id;
            auto signal_name = gsmv.signal_name;
            auto switch_name = gsmv.switch_name;
            std::vector<SignalMultiplexerValue::Range> value_ranges;
            for (auto grange : gsmv.value_ranges)
            {
                value_ranges.push_back({grange.from, grange.to});
            }
            auto smv = SignalMultiplexerValue::create(
                std::move(switch_name),
                std::move(value_ranges));
            result.emplace_back(std::move(smv));
        }
    }
    return result;
}
static auto getSignals(const GNetwork& gnet, const GMessage& m)
{
    std::vector<std::unique_ptr<Signal>> result;
    for (const GSignal& s : m.signals)
    {
        std::vector<std::string> receivers;
        auto attribute_values = getAttributeValues(gnet, m, s);
        auto value_descriptions = getValueDescriptions(gnet, m, s);
        auto extended_value_type = getSignalExtendedValueType(gnet, m, s);
        auto multiplexer_indicator = Signal::Multiplexer::NoMux;
        auto comment = getComment(gnet, m, s);
        auto signal_multiplexer_values = getSignalMultiplexerValues(gnet, m, s);
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
            , extended_value_type
            , std::move(signal_multiplexer_values));
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
static auto getMessageTransmitters(const GNetwork& gnet, const GMessage& m)
{
    std::vector<std::string> result;
    auto iter_mt = std::find_if(gnet.message_transmitters.begin(), gnet.message_transmitters.end(),
        [&](const GMessageTransmitter& mt)
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
static auto getAttributeValues(const GNetwork& gnet, const GMessage& m)
{
    std::vector<std::unique_ptr<Attribute>> result;
    for (const auto& vav : gnet.attribute_values)
    {
        if (auto pam = std::get_if<GAttributeMessage>(&vav))
        {
            const auto& av = *pam;
            if (av.message_id == m.id)
            {
                auto value = av.value;
                auto attribute = Attribute::create(std::string(av.attribute_name), AttributeDefinition::ObjectType::Message, std::move(value));
                result.push_back(std::move(attribute));
            }
        }
    }
    return result;
}
static auto getComment(const GNetwork& gnet, const GMessage& m)
{
    std::string result;
    for (const auto& c : gnet.comments)
    {
        if (auto pcm = std::get_if<GCommentMessage>(&c);
            pcm && pcm->message_id == m.id)
        {
            result = std::get<GCommentMessage>(c).comment;
            break;
        }
    }
    return result;
}
static auto getMessages(const GNetwork& gnet)
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
static auto getValueDescriptions(const GNetwork& gnet, const GEnvironmentVariable& ev)
{
    std::vector<std::tuple<int64_t, std::string>> result;
    for (const auto& vds : gnet.value_descriptions)
    {
        if (auto pvde = std::get_if<GValueDescriptionEnvVar>(&vds.description);
            pvde && pvde->env_var_name == ev.name)
        {
            result = std::get<GValueDescriptionEnvVar>(vds.description).value_descriptions;
            break;
        }
    }
    return result;
}
static auto getAttributeValues(const GNetwork& gnet, const GEnvironmentVariable& ev)
{
    std::vector<std::unique_ptr<Attribute>> result;
    for (const auto& vav : gnet.attribute_values)
    {
        if (auto paev = std::get_if<GAttributeEnvVar>(&vav))
        {
            const auto& av = *paev;
            if (av.env_var_name == ev.name)
            {
                auto value = av.value;
                auto attribute = Attribute::create(std::string(av.attribute_name), AttributeDefinition::ObjectType::EnvironmentVariable, std::move(value));
                result.push_back(std::move(attribute));
            }
        }
    }
    return result;
}
static auto getComment(const GNetwork& gnet, const GEnvironmentVariable& ev)
{
    std::string result;
    for (const auto& c : gnet.comments)
    {
        if (auto pce = std::get_if<GCommentEnvVar>(&c);
            pce && pce->env_var_name == ev.name)
        {
            result = std::get<GCommentEnvVar>(c).comment;
            break;
        }
    }
    return result;
}
static auto getEnvironmentVariables(const GNetwork& gnet)
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
static auto getAttributeDefinitions(const GNetwork& gnet)
{
    std::vector<std::unique_ptr<AttributeDefinition>> result;
    struct VisitorValueType
    {
        AttributeDefinition::value_type_t operator()(const GAttributeValueTypeInt& cn)
        {
            AttributeDefinition::ValueTypeInt vt;
            vt.minimum = cn.minimum;
            vt.maximum = cn.maximum;
            return vt;
        }
        AttributeDefinition::value_type_t operator()(const GAttributeValueTypeHex& cn)
        {
            AttributeDefinition::ValueTypeHex vt;
            vt.minimum = cn.minimum;
            vt.maximum = cn.maximum;
            return vt;
        }
        AttributeDefinition::value_type_t operator()(const GAttributeValueTypeFloat& cn)
        {
            AttributeDefinition::ValueTypeFloat vt;
            vt.minimum = cn.minimum;
            vt.maximum = cn.maximum;
            return vt;
        }
        AttributeDefinition::value_type_t operator()(const GAttributeValueTypeString& cn)
        {
            return AttributeDefinition::ValueTypeString();
        }
        AttributeDefinition::value_type_t operator()(const GAttributeValueTypeEnum& cn)
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
        std::visit(vvt, cvt.value);
        auto nad = AttributeDefinition::create(std::move(std::string(ad.name)), object_type, std::visit(vvt, cvt.value));
        result.push_back(std::move(nad));
    }
    return result;
}
static auto getAttributeDefaults(const GNetwork& gnet)
{
    std::vector<std::unique_ptr<Attribute>> result;
    for (auto& ad : gnet.attribute_defaults)
    {
        auto nad = Attribute::create(std::string(ad.name), AttributeDefinition::ObjectType::Network, ad.value);
        result.push_back(std::move(nad));
    }
    return result;
}
static auto getAttributeValues(const GNetwork& gnet)
{
    std::vector<std::unique_ptr<Attribute>> result;
    for (const auto& av : gnet.attribute_values)
    {
        if (auto pan = std::get_if<GAttributeNetwork>(&av))
        {
            auto av_ = *pan;
            auto attribute = Attribute::create(
                std::string(av_.attribute_name)
                , AttributeDefinition::ObjectType::Network
                , std::move(av_.value));
            result.push_back(std::move(attribute));
        }
    }
    return result;
}
static auto getComment(const GNetwork& gnet)
{
    std::string result;
    for (const auto& c : gnet.comments)
    {
        if (auto pcn = std::get_if<GCommentNetwork>(&c))
        {
            result = pcn->comment;
            break;
        }
    }
    return result;
}

std::unique_ptr<Network> DBCAST2Network(const GNetwork& gnet)
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
    std::unique_ptr<Network> result;
    std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    GNetwork gnet;
    DBCIterator iter(str.c_str());
    if (DBCParser::ParseNetwork(iter, gnet))
    {
        result = DBCAST2Network(gnet);
    }
    return result;
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
