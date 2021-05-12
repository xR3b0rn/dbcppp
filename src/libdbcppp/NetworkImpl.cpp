#include <fstream>
#include <iomanip>
#include "../../include/dbcppp/Network.h"
#include "NetworkImpl.h"

using namespace dbcppp;

std::unique_ptr<Network> Network::create(
      std::string&& version
    , std::vector<std::string>&& new_symbols
    , std::unique_ptr<BitTiming>&& bit_timing
    , std::vector<std::unique_ptr<Node>>&& nodes
    , std::vector<std::unique_ptr<ValueTable>>&& value_tables
    , std::vector<std::unique_ptr<Message>>&& messages
    , std::vector<std::unique_ptr<EnvironmentVariable>>&& environment_variables
    , std::vector<std::unique_ptr<AttributeDefinition>>&& attribute_definitions
    , std::vector<std::unique_ptr<Attribute>>&& attribute_defaults
    , std::vector<std::unique_ptr<Attribute>>&& attribute_values
    , std::string&& comment)
{
    BitTimingImpl bt = std::move(static_cast<BitTimingImpl&>(*bit_timing));
    bit_timing.reset(nullptr);
    std::vector<NodeImpl> ns;
    std::vector<ValueTableImpl> vts;
    std::vector<MessageImpl> ms;
    std::vector<EnvironmentVariableImpl> evs;
    std::vector<AttributeDefinitionImpl> ads;
    std::vector<AttributeImpl> avds;
    std::vector<AttributeImpl> avs;
    for (auto& n : nodes)
    {
        ns.push_back(std::move(static_cast<NodeImpl&>(*n)));
        n.reset(nullptr);
    }
    for (auto& vt : value_tables)
    {
        vts.push_back(std::move(static_cast<ValueTableImpl&>(*vt)));
        vt.reset(nullptr);
    }
    for (auto& m : messages)
    {
        ms.push_back(std::move(static_cast<MessageImpl&>(*m)));
        m.reset(nullptr);
    }
    for (auto& ev : environment_variables)
    {
        evs.push_back(std::move(static_cast<EnvironmentVariableImpl&>(*ev)));
        ev.reset(nullptr);
    }
    for (auto& ad : attribute_definitions)
    {
        ads.push_back(std::move(static_cast<AttributeDefinitionImpl&>(*ad)));
        ad.reset(nullptr);
    }
    for (auto& ad : attribute_defaults)
    {
        avds.push_back(std::move(static_cast<AttributeImpl&>(*ad)));
        ad.reset(nullptr);
    }
    for (auto& av : attribute_values)
    {
        avs.push_back(std::move(static_cast<AttributeImpl&>(*av)));
        av.reset(nullptr);
    }
    return std::make_unique<NetworkImpl>(
          std::move(version)
        , std::move(new_symbols)
        , std::move(bt)
        , std::move(ns)
        , std::move(vts)
        , std::move(ms)
        , std::move(evs)
        , std::move(ads)
        , std::move(avds)
        , std::move(avs)
        , std::move(comment));
}

NetworkImpl::NetworkImpl(
      std::string&& version
    , std::vector<std::string>&& new_symbols
    , BitTimingImpl&& bit_timing
    , std::vector<NodeImpl>&& nodes
    , std::vector<ValueTableImpl>&& value_tables
    , std::vector<MessageImpl>&& messages
    , std::vector<EnvironmentVariableImpl>&& environment_variables
    , std::vector<AttributeDefinitionImpl>&& attribute_definitions
    , std::vector<AttributeImpl>&& attribute_defaults
    , std::vector<AttributeImpl>&& attribute_values
    , std::string&& comment)

    : _version(std::move(version))
    , _new_symbols(std::move(new_symbols))
    , _bit_timing(std::move(bit_timing))
    , _nodes(std::move(nodes))
    , _value_tables(std::move(value_tables))
    , _messages(std::move(messages))
    , _environment_variables(std::move(environment_variables))
    , _attribute_definitions(std::move(attribute_definitions))
    , _attribute_defaults(std::move(attribute_defaults))
    , _attribute_values(std::move(attribute_values))
    , _comment(std::move(comment))
{}
std::unique_ptr<Network> NetworkImpl::clone() const
{
    return std::make_unique<NetworkImpl>(*this);
}
const std::string& NetworkImpl::getVersion() const
{
    return _version;
}
bool NetworkImpl::hasNewSymbol(const std::string& name) const
{
    return std::find(_new_symbols.begin(), _new_symbols.end(), name) != _new_symbols.end();
}
void NetworkImpl::forEachNewSymbol(std::function<void(const std::string&)> cb) const
{
    for (const auto& ns : _new_symbols)
    {
        cb(ns);
    }
}
const BitTiming& NetworkImpl::getBitTiming() const
{
    return _bit_timing;
}
const Node* NetworkImpl::getNodeByName(const std::string& name) const
{
    const Node* result = nullptr;
    auto iter = std::find_if(_nodes.begin(), _nodes.end(),
        [&](const NodeImpl& other) { return other.getName() == name; });
    if (iter != _nodes.end())
    {
        result = &*iter;
    }
    return result;
}
const Node* NetworkImpl::findNode(std::function<bool(const Node&)>&& pred) const
{
    const Node* result = nullptr;
    for (const auto& n : _nodes)
    {
        if (pred(n))
        {
            result = &n;
            break;
        }
    }
    return result;
}
void NetworkImpl::forEachNode(std::function<void(const Node&)>&& cb) const
{
    for (const auto& n : _nodes)
    {
        cb(n);
    }
}
const ValueTable* NetworkImpl::getValueTableByName(const std::string& name) const
{
    const ValueTable* result = nullptr;
    auto iter = std::find_if(_value_tables.begin(), _value_tables.end(),
        [&](const ValueTableImpl& other) { return other.getName() == name; });
    if (iter != _value_tables.end())
    {
        result = &*iter;
    }
    return result;
}
const ValueTable* NetworkImpl::findValueTable(std::function<bool(const ValueTable&)>&& pred) const
{
    const ValueTable* result = nullptr;
    for (const auto& vt : _value_tables)
    {
        if (pred(vt))
        {
            result = &vt;
            break;
        }
    }
    return result;
}
void NetworkImpl::forEachValueTable(std::function<void(const ValueTable&)>&& cb) const
{
    for (const auto& vt : _value_tables)
    {
        cb(vt);
    }
}
const Message* NetworkImpl::getMessageById(uint64_t id) const
{
    const Message* result = nullptr;
    auto iter = std::find_if(_messages.begin(), _messages.end(),
        [&](const MessageImpl& other) { return other.getId() == id; });
    if (iter != _messages.end())
    {
        result = &*iter;
    }
    return result;
}
const Message* NetworkImpl::findMessage(std::function<bool(const Message&)>&& pred) const
{
    const Message* result = nullptr;
    for (const auto& m : _messages)
    {
        if (pred(m))
        {
            result = &m;
            break;
        }
    }
    return result;
}
void NetworkImpl::forEachMessage(std::function<void(const Message&)>&& cb) const
{
    for (const auto& m : _messages)
    {
        cb(m);
    }
}
const EnvironmentVariable* NetworkImpl::getEnvironmentVariableByName(const std::string& name) const
{
    const EnvironmentVariable* result = nullptr;
    auto iter = std::find_if(_environment_variables.begin(), _environment_variables.end(),
        [&](const EnvironmentVariableImpl& other) { return other.getName() == name; });
    if (iter != _environment_variables.end())
    {
        result = &*iter;
    }
    return result;
}
const EnvironmentVariable* NetworkImpl::findEnvironmentVariable(std::function<bool(const EnvironmentVariable&)>&& pred) const
{
    const EnvironmentVariable* result = nullptr;
    for (const auto& ev : _environment_variables)
    {
        if (pred(ev))
        {
            result = &ev;
            break;
        }
    }
    return result;
}
void NetworkImpl::forEachEnvironmentVariable(std::function<void(const EnvironmentVariable&)>&& cb) const
{
    for (const auto& ev : _environment_variables)
    {
        cb(ev);
    }
}
const AttributeDefinition* NetworkImpl::getAttributeDefinitionByName(const std::string& name) const
{
    const AttributeDefinition* result = nullptr;
    auto iter = std::find_if(_attribute_definitions.begin(), _attribute_definitions.end(),
        [&](const AttributeDefinitionImpl& other) { return other.getName() == name; });
    if (iter != _attribute_definitions.end())
    {
        result = &*iter;
    }
    return result;
}
const AttributeDefinition* NetworkImpl::findAttributeDefinition(std::function<bool(const AttributeDefinition&)>&& pred) const
{
    const AttributeDefinition* result = nullptr;
    for (const auto& ad : _attribute_definitions)
    {
        if (pred(ad))
        {
            result = &ad;
            break;
        }
    }
    return result;
}
void NetworkImpl::forEachAttributeDefinition(std::function<void(const AttributeDefinition&)>&& cb) const
{
    for (const auto& ad : _attribute_definitions)
    {
        cb(ad);
    }
}
const Attribute* NetworkImpl::getAttributeDefaultByName(const std::string& name) const
{
    const Attribute* result = nullptr;
    auto iter = std::find_if(_attribute_defaults.begin(), _attribute_defaults.end(),
        [&](const AttributeImpl& other) { return other.getName() == name; });
    if (iter != _attribute_defaults.end())
    {
        result = &*iter;
    }
    return result;
}
const Attribute* NetworkImpl::findAttributeDefault(std::function<bool(const Attribute&)>&& pred) const
{
    const Attribute* result = nullptr;
    for (const auto& ad : _attribute_defaults)
    {
        if (pred(ad))
        {
            result = &ad;
            break;
        }
    }
    return result;
}
void NetworkImpl::forEachAttributeDefault(std::function<void(const Attribute&)>&& cb) const
{
    for (const auto& ad : _attribute_defaults)
    {
        cb(ad);
    }
}
const Attribute* NetworkImpl::getAttributeValueByName(const std::string& name) const
{
    const Attribute* result = nullptr;
    auto iter = std::find_if(_attribute_values.begin(), _attribute_values.end(),
        [&](const AttributeImpl& other) { return other.getName() == name; });
    if (iter != _attribute_values.end())
    {
        result = &*iter;
    }
    return result;
}
const Attribute* NetworkImpl::findAttributeValue(std::function<bool(const Attribute&)>&& pred) const
{
    const Attribute* result = nullptr;
    for (const auto& av : _attribute_values)
    {
        if (pred(av))
        {
            result = &av;
            break;
        }
    }
    return result;
}
void NetworkImpl::forEachAttributeValue(std::function<void(const Attribute&)>&& cb) const
{
    for (const auto& av : _attribute_values)
    {
        cb(av);
    }
}
const std::string& NetworkImpl::getComment() const
{
    return _comment;
}
const Message* NetworkImpl::findParentMessage(const Signal* sig) const
{
    const Message* result = nullptr;
    for (const auto& msg : _messages)
    {
        auto iter = std::find_if(msg.signals().begin(), msg.signals().end(),
            [&](const SignalImpl& other) { return &other == sig; });
        if (iter != msg.signals().end())
        {
            result = &msg;
            break;
        }
    }
    return result;
}
std::string& NetworkImpl::version()
{
    return _version;
}
std::vector<std::string>& NetworkImpl::newSymbols()
{
    return _new_symbols;
}
BitTimingImpl& NetworkImpl::bitTiming()
{
    return _bit_timing;
}
std::vector<NodeImpl>& NetworkImpl::nodes()
{
    return _nodes;
}
std::vector<ValueTableImpl>& NetworkImpl::valueTables()
{
    return _value_tables;
}
std::vector<MessageImpl>& NetworkImpl::messages()
{
    return _messages;
}
std::vector<EnvironmentVariableImpl>& NetworkImpl::environmentVariables()
{
    return _environment_variables;
}
std::vector<AttributeDefinitionImpl>& NetworkImpl::attributeDefinitions()
{
    return _attribute_definitions;
}
std::vector<AttributeImpl>& NetworkImpl::attributeDefaults()
{
    return _attribute_defaults;
}
std::vector<AttributeImpl>& NetworkImpl::attributeValues()
{
    return _attribute_values;
}
std::string& NetworkImpl::comment()
{
    return _comment;
}
void Network::merge(std::unique_ptr<Network>&& other)
{
    auto& self = static_cast<NetworkImpl&>(*this);
    auto& o = static_cast<NetworkImpl&>(*other);
    for (auto& ns : o.newSymbols())
    {
        self.newSymbols().push_back(std::move(ns));
    }
    for (auto& n : o.nodes())
    {
        self.nodes().push_back(std::move(n));
    }
    for (auto& vt : o.valueTables())
    {
        self.valueTables().push_back(std::move(vt));
    }
    for (auto& m : o.messages())
    {
        self.messages().push_back(std::move(m));
    }
    for (auto& ev : o.environmentVariables())
    {
        self.environmentVariables().push_back(std::move(ev));
    }
    for (auto& ad : o.attributeDefinitions())
    {
        self.attributeDefinitions().push_back(std::move(ad));
    }
    for (auto& ad : o.attributeDefaults())
    {
        self.attributeDefaults().push_back(std::move(ad));
    }
    for (auto& av : o.attributeValues())
    {
        self.attributeValues().push_back(std::move(av));
    }
    other.reset(nullptr);
}
bool NetworkImpl::operator==(const Network& rhs) const
{
    bool result = true;
    result &= _version == rhs.getVersion();
    rhs.forEachNewSymbol(
        [&](const std::string& new_symbol)
        {
            result &= std::find(_new_symbols.begin(), _new_symbols.end(), new_symbol) != _new_symbols.end();
        });
    result &= _bit_timing == rhs.getBitTiming();
    rhs.forEachNode(
        [&](const dbcppp::Node& node)
        {
            result &= std::find(_nodes.begin(), _nodes.end(), node) != _nodes.end();
        });
    rhs.forEachValueTable(
        [&](const dbcppp::ValueTable& value_table)
        {
            result &= std::find(_value_tables.begin(), _value_tables.end(), value_table) != _value_tables.end();
        });
    rhs.forEachMessage(
        [&](const dbcppp::Message& message)
        {
            result &= std::find(_messages.begin(), _messages.end(), message) != _messages.end();
        });
    rhs.forEachEnvironmentVariable(
        [&](const dbcppp::EnvironmentVariable& env_var)
        {
            result &= std::find(_environment_variables.begin(), _environment_variables.end(), env_var) != _environment_variables.end();
        });
    rhs.forEachAttributeDefinition(
        [&](const dbcppp::AttributeDefinition& attr_def)
        {
            result &= std::find(_attribute_definitions.begin(), _attribute_definitions.end(), attr_def) != _attribute_definitions.end();
        });
    rhs.forEachAttributeDefault(
        [&](const dbcppp::Attribute& attr)
        {
            result &= std::find(_attribute_defaults.begin(), _attribute_defaults.end(), attr) != _attribute_defaults.end();
        });
    rhs.forEachAttributeValue(
        [&](const dbcppp::Attribute& attr)
        {
            result &= std::find(_attribute_values.begin(), _attribute_values.end(), attr) != _attribute_values.end();
        });
    result &= _comment == rhs.getComment();
    return result;
}
bool NetworkImpl::operator!=(const Network& rhs) const
{
    return !(*this == rhs);
}

std::map<std::string, std::unique_ptr<Network>> Network::loadNetworkFromFile(const std::filesystem::path& filename)
{
    auto result = std::map<std::string, std::unique_ptr<Network>>();
    auto is = std::ifstream(filename);
    if (!is.is_open())
    {
        std::cout << "Error: Could not open file " << filename << "\n";
    }
    else if (filename.extension() == ".dbc")
    {
        result.insert(std::make_pair("", loadDBCFromIs(is)));
    }
    else if (filename.extension() == ".kcd")
    {
        result = loadKCDFromIs(is);
    }
    return std::move(result);
}