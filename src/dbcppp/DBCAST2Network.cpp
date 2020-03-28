
#include <boost/log/trivial.hpp>
#include <iterator>
#include "../../include/dbcppp/Network.h"
#include "DBC_Grammar.h"

using namespace dbcppp;

static auto getVersion(const G_Network& gnet)
{
    return gnet.version.version;
}
static auto getNewSymbols(const G_Network& gnet)
{
    std::set<std::string> nodes;
    for (const auto& ns : gnet.new_symbols)
    {
        nodes.insert(ns);
    }
    return nodes;
}
static auto getSignalType(const G_Network& gnet, const G_ValueTable& vt)
{
    boost::optional<std::unique_ptr<SignalType>> result;
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
    std::map<std::string, std::unique_ptr<ValueTable>> result;
    for (auto& vt : gnet.value_tables)
    {
        auto sig_type = getSignalType(gnet, vt);
        auto copy_ved = vt.value_encoding_descriptions;
        auto nvt = ValueTable::create(std::string(vt.name), std::move(sig_type), std::move(copy_ved));
        result.insert(std::make_pair(vt.name, std::move(nvt)));
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
static auto getAttributeValues(const G_Network& gnet, const G_Node& n)
{
    std::map<std::string, std::unique_ptr<Attribute>> result;
    for (const variant_attribute_t& av : gnet.attribute_values)
    {
        if (av.type() == typeid(G_AttributeNode) &&
            boost::get<G_AttributeNode>(av).node_name == n.name)
        {
            auto name = boost::get<G_AttributeNode>(av).attribute_name;
            auto value = boost::get<G_AttributeNode>(av).value;
            auto attribute = Attribute::create(std::move(name), AttributeDefinition::ObjectType::Node, std::move(value));
            result.insert(std::make_pair(name, std::move(attribute)));
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
            return c.type() == typeid(G_CommentNode) &&
                boost::get<G_CommentNode>(c).node_name == n.name;
        });
    if (iter_comment != gnet.comments.end())
    {
        result = boost::get<G_CommentNode>(*iter_comment).comment;
    }
    return result;
}
static auto getNodes(const G_Network& gnet)
{
    std::map<std::string, std::unique_ptr<Node>> result;
    for (const auto& n : gnet.nodes)
    {
        auto comment = getComment(gnet, n);
        auto attribute_values = getAttributeValues(gnet, n);
        auto nn = Node::create(std::string(n.name), std::move(comment), std::move(attribute_values));
        result.insert(std::make_pair(n.name, std::move(nn)));
    }
    return result;
}
static auto getAttributeValues(const G_Network& gnet, const G_Message& m, const G_Signal& s)
{
    std::map<std::string, std::unique_ptr<Attribute>> result;
    for (const auto& vav : gnet.attribute_values)
    {
        if (vav.type() == typeid(G_AttributeSignal))
        {
            const auto& av = boost::get<G_AttributeSignal>(vav);
            if (av.message_id == m.id && av.signal_name == s.name)
            {
                auto value = av.value;
                auto attribute = Attribute::create(std::string(av.attribute_name), AttributeDefinition::ObjectType::Signal, std::move(value));
                result.insert(std::make_pair(av.attribute_name, std::move(attribute)));
            }
        }
    }
    return result;
}
static auto getValueDescriptions(const G_Network& gnet, const G_Message& m, const G_Signal& s)
{
    std::map<double, std::string> result;
    for (const auto& vds : gnet.value_descriptions)
    {
        if (vds.description.type() == typeid(G_ValueDescriptionSignal) &&
            boost::get<G_ValueDescriptionSignal>(vds.description).message_id == m.id &&
            boost::get<G_ValueDescriptionSignal>(vds.description).signal_name == s.name)
        {
            result = boost::get<G_ValueDescriptionSignal>(vds.description).value_descriptions;
        }
    }
    return result;
}
static auto getComment(const G_Network& gnet, const G_Message& m, const G_Signal& s)
{
    std::string result;
    for (const auto& c : gnet.comments)
    {
        if (c.type() == typeid(G_CommentSignal) &&
            boost::get<G_CommentSignal>(c).message_id == m.id &&
            boost::get<G_CommentSignal>(c).signal_name == s.name)
        {
            result = boost::get<G_CommentSignal>(c).comment;
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
    std::map<std::string, std::unique_ptr<Signal>> result;
    for (const G_Signal& s : m.signals)
    {
        std::set<std::string> receivers;
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
            receivers.insert(n);
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
        switch (ns->getError())
        {
        case SignalImpl::ErrorCode::SignalExceedsMessageSize:
            std::cout << "Warning: The signals '" << m.name << "::" << s.name << "'"
                << " start_bit + bit_size exceeds the byte size of the message! Ignoring this error will lead to garbage data when using the decode function of this signal." << std::endl;
            break;
        case SignalImpl::ErrorCode::WrongBitSizeForExtendedDataType:
            std::cout << "Warning: The signals '" << m.name << "::" << s.name << "'"
                << " bit_size does not fit the bit size of the specified ExtendedValueType." << std::endl;
            break;
        case SignalImpl::ErrorCode::MaschinesFloatEncodingNotSupported:
            std::cout << "Warning: Signal '" << m.name << "::" << s.name << "'"
                << " This warning appears when a signal uses type float but the system this programm is running on does not uses IEEE 754 encoding for floats." << std::endl;
            break;
        case SignalImpl::ErrorCode::MaschinesDoubleEncodingNotSupported:
            std::cout << "Warning: Signal '" << m.name << "::" << s.name << "'"
                << " This warning appears when a signal uses type double but the system this programm is running on does not uses IEEE 754 encoding for doubles." << std::endl;
            break;
        }
        result.insert(std::make_pair(s.name, std::move(ns)));
    }
    return result;
}
static auto getMessageTransmitters(const G_Network& gnet, const G_Message& m)
{
    std::set<std::string> result;
    auto iter_mt = std::find_if(gnet.message_transmitters.begin(), gnet.message_transmitters.end(),
        [&](const G_MessageTransmitter& mt)
        {
            return mt.id == m.id;
        });
    if (iter_mt != gnet.message_transmitters.end())
    {
        for (const auto& t : iter_mt->transmitters)
        {
            result.insert(t);
        }
    }
    return result;
}
static auto getAttributeValues(const G_Network& gnet, const G_Message& m)
{
    std::map<std::string, std::unique_ptr<Attribute>> result;
    for (const auto& vav : gnet.attribute_values)
    {
        if (vav.type() == typeid(G_AttributeMessage))
        {
            const auto& av = boost::get<G_AttributeMessage>(vav);
            if (av.message_id == m.id)
            {
                auto value = av.value;
                auto attribute = Attribute::create(std::string(av.attribute_name), AttributeDefinition::ObjectType::Message, std::move(value));
                result.insert(std::make_pair(av.attribute_name, std::move(attribute)));
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
        if (c.type() == typeid(G_CommentMessage) &&
            boost::get<G_CommentMessage>(c).message_id == m.id)
        {
            result = boost::get<G_CommentMessage>(c).comment;
            break;
        }
    }
    return result;
}
static auto getMessages(const G_Network& gnet)
{
    std::unordered_map<uint64_t, std::unique_ptr<Message>> result;
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
        result.insert(std::make_pair(m.id, std::move(msg)));
    }
    return result;
}
static auto getValueDescriptions(const G_Network& gnet, const G_EnvironmentVariable& ev)
{
    std::map<double, std::string> result;
    for (const auto& vds : gnet.value_descriptions)
    {
        if (vds.description.type() == typeid(G_ValueDescriptionEnvVar) &&
            boost::get<G_ValueDescriptionEnvVar>(vds.description).env_var_name == ev.name)
        {
            result = boost::get<G_ValueDescriptionEnvVar>(vds.description).value_descriptions;
            break;
        }
    }
    return result;
}
static auto getAttributeValues(const G_Network& gnet, const G_EnvironmentVariable& ev)
{
    std::map<std::string, std::unique_ptr<Attribute>> result;
    for (const auto& vav : gnet.attribute_values)
    {
        if (vav.type() == typeid(G_AttributeEnvVar))
        {
            const auto& av = boost::get<G_AttributeEnvVar>(vav);
            if (av.env_var_name == ev.name)
            {
                auto value = av.value;
                auto attribute = Attribute::create(std::string(av.attribute_name), AttributeDefinition::ObjectType::EnvironmentVariable, std::move(value));
                result.insert(std::make_pair(av.attribute_name, std::move(attribute)));
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
        if (c.type() == typeid(G_CommentEnvVar) &&
            boost::get<G_CommentEnvVar>(c).env_var_name == ev.name)
        {
            result = boost::get<G_CommentEnvVar>(c).comment;
            break;
        }
    }
    return result;
}
static auto getEnvironmentVariables(const G_Network& gnet)
{
    std::map<std::string, std::unique_ptr<EnvironmentVariable>> result;
    for (const auto& ev : gnet.environment_variables)
    {
        EnvironmentVariable::VarType var_type;
        EnvironmentVariable::AccessType access_type;
        std::set<std::string> access_nodes;
        auto value_descriptions = getValueDescriptions(gnet, ev);
        auto attribute_values = getAttributeValues(gnet, ev);
        auto comment = getComment(gnet, ev);
        uint64_t data_size = 0;
        for (const auto& n : ev.access_nodes)
        {
            access_nodes.insert(n);
        }
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
        result.insert(std::make_pair(ev.name, std::move(env_var)));
    }
    return result;
}
static auto getAttributeDefinitions(const G_Network& gnet)
{
    std::map<std::string, std::unique_ptr<AttributeDefinition>> result;
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
        boost::apply_visitor(vvt, cvt.value);
        auto nad = AttributeDefinition::create(std::move(std::string(ad.name)), object_type, boost::apply_visitor(vvt, cvt.value));
        result.insert(std::make_pair(ad.name, std::move(nad)));
    }
    return result;
}
static auto getAttributeDefaults(const G_Network& gnet)
{
    std::map<std::string, std::unique_ptr<Attribute>> result;
    for (auto& ad : gnet.attribute_defaults)
    {
        auto nad = Attribute::create(std::string(ad.name), AttributeDefinition::ObjectType::Network, ad.value);
        result.insert(std::make_pair(ad.name, std::move(nad)));
    }
    return result;
}
static auto getAttributeValues(const G_Network& gnet)
{
    std::map<std::string, std::unique_ptr<Attribute>> result;
    for (const auto& av : gnet.attribute_values)
    {
        if (av.type() == typeid(G_AttributeNetwork))
        {
            auto av_ = boost::get<G_AttributeNetwork>(av);
            auto attribute = Attribute::create(
                  std::string(av_.attribute_name)
                , AttributeDefinition::ObjectType::Network
                , std::move(av_.value));
            result.insert(std::make_pair(av_.attribute_name, std::move(attribute)));
        }
    }
    return result;
}
static auto getComment(const G_Network& gnet)
{
    std::string result;
    for (const auto& c : gnet.comments)
    {
        if (c.type() == typeid(G_CommentNetwork))
        {
            result = boost::get<G_CommentNetwork>(c).comment;
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

std::unique_ptr<Network> Network::fromDBC(std::istream& is)
{
    std::unique_ptr<Network> result;
    std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    auto begin{str.begin()}, end{str.end()};
    NetworkGrammar<std::string::iterator> g(begin);
    G_Network gnet;
    bool succeeded = phrase_parse(begin, end, g, boost::spirit::ascii::space, gnet);
    if (succeeded && (begin == end))
    {
        result = DBCAST2Network(gnet);
    }
    else
    {
        auto[line, column] = getErrPos(str.begin(), begin);
        std::cout << line << ":" << column << " Error! Unexpected token near here!" << std::endl;
    }
    return result;
}
std::unique_ptr<Network> Network::fromDBC(std::istream& is, std::unique_ptr<Network> network)
{
    auto other = fromDBC(is);
    network->merge(std::move(other));
    return std::move(network);
}
extern "C"
{
    DBCPPP_API const dbcppp_Network* dbcppp_NetworkLoadDBCFromFile(const char* filename)
    {
        std::unique_ptr<Network> result;
        std::ifstream is(filename);
        if (is.is_open())
        {
            std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
            auto begin{str.begin()}, end{str.end()};
            NetworkGrammar<std::string::iterator> g(begin);
            G_Network gnet;
            bool succeeded = phrase_parse(begin, end, g, boost::spirit::ascii::space, gnet);
            if (succeeded && (begin == end))
            {
                result = DBCAST2Network(gnet);
            }
            else
            {
                auto[line, column] = getErrPos(str.begin(), begin);
                std::cout << line << ":" << column << " Error! Unexpected token near here!" << std::endl;
            }
        }
        else
        {
            std::cout << "Error! Couldn't find \"" << filename << "\"" << std::endl;
        }
        return reinterpret_cast<const dbcppp_Network*>(result.release());
    }
}
