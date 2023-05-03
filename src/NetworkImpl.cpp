#include <fstream>
#include <iomanip>
#include "dbcppp/Network.h"
#include "NetworkImpl.h"

using namespace dbcppp;

std::unique_ptr<INetwork> INetwork::Create(
      std::string&& version
    , std::vector<std::string>&& new_symbols
    , std::unique_ptr<IBitTiming>&& bit_timing
    , std::vector<std::unique_ptr<INode>>&& nodes
    , std::vector<std::unique_ptr<IValueTable>>&& value_tables
    , std::vector<std::unique_ptr<IMessage>>&& messages
    , std::vector<std::unique_ptr<IEnvironmentVariable>>&& environment_variables
    , std::vector<std::unique_ptr<IAttributeDefinition>>&& attribute_definitions
    , std::vector<std::unique_ptr<IAttribute>>&& attribute_defaults
    , std::vector<std::unique_ptr<IAttribute>>&& attribute_values
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
std::unique_ptr<INetwork> NetworkImpl::Clone() const
{
    return std::make_unique<NetworkImpl>(*this);
}
const std::string& NetworkImpl::Version() const
{
    return _version;
}
const std::string& NetworkImpl::NewSymbols_Get(std::size_t i) const
{
    return _new_symbols[i];
}
uint64_t NetworkImpl::NewSymbols_Size() const
{
    return _new_symbols.size();
}
const IBitTiming& NetworkImpl::BitTiming() const
{
    return _bit_timing;
}
const INode& NetworkImpl::Nodes_Get(std::size_t i) const
{
    return _nodes[i];
}
uint64_t NetworkImpl::Nodes_Size() const
{
    return _nodes.size();
}
const IValueTable& NetworkImpl::ValueTables_Get(std::size_t i) const
{
    return _value_tables[i];
}
uint64_t NetworkImpl::ValueTables_Size() const
{
    return _value_tables.size();
}
const IMessage& NetworkImpl::Messages_Get(std::size_t i) const
{
    return _messages[i];
}
uint64_t NetworkImpl::Messages_Size() const
{
    return _messages.size();
}
const IEnvironmentVariable& NetworkImpl::EnvironmentVariables_Get(std::size_t i) const
{
    return _environment_variables[i];
}
uint64_t NetworkImpl::EnvironmentVariables_Size() const
{
    return _environment_variables.size();
}
const IAttributeDefinition& NetworkImpl::AttributeDefinitions_Get(std::size_t i) const
{
    return _attribute_definitions[i];
}
uint64_t NetworkImpl::AttributeDefinitions_Size() const
{
    return _attribute_definitions.size();
}
const IAttribute& NetworkImpl::AttributeDefaults_Get(std::size_t i) const
{
    return _attribute_defaults[i];
}
uint64_t NetworkImpl::AttributeDefaults_Size() const
{
    return _attribute_defaults.size();
}
const IAttribute& NetworkImpl::AttributeValues_Get(std::size_t i) const
{
    return _attribute_values[i];
}
uint64_t NetworkImpl::AttributeValues_Size() const
{
    return _attribute_values.size();
}
const std::string& NetworkImpl::Comment() const
{
    return _comment;
}
const IMessage* NetworkImpl::ParentMessage(const ISignal* sig) const
{
    const IMessage* parent = nullptr;
    for (const auto& msg : _messages)
    {
        auto iter = std::find_if(msg.signals().begin(), msg.signals().end(),
            [&](const SignalImpl& other) { return &other == sig; });
        if (iter != msg.signals().end())
        {
            parent = &msg;
            break;
        }
    }
    return parent;
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
void INetwork::Merge(std::unique_ptr<INetwork>&& other)
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
bool NetworkImpl::operator==(const INetwork& rhs) const
{
    bool equal = true;
    equal &= _version == rhs.Version();
    for (const auto& new_symbol : _new_symbols)
    {
        equal &= std::find(_new_symbols.begin(), _new_symbols.end(), new_symbol) != _new_symbols.end();
    }
    equal &= _bit_timing == rhs.BitTiming();
    for (const auto& node : rhs.Nodes())
    {
        equal &= std::find(_nodes.begin(), _nodes.end(), node) != _nodes.end();
    }
    for (const auto& value_table : rhs.ValueTables())
    {
        equal &= std::find(_value_tables.begin(), _value_tables.end(), value_table) != _value_tables.end();
    }
    for (const auto& message : rhs.Messages())
    {
        equal &= std::find(_messages.begin(), _messages.end(), message) != _messages.end();
    }
    for (const auto& env_var : rhs.EnvironmentVariables())
    {
        equal &= std::find(_environment_variables.begin(), _environment_variables.end(), env_var) != _environment_variables.end();
    }
    for (const auto& attr_def : rhs.AttributeDefinitions())
    {
        equal &= std::find(_attribute_definitions.begin(), _attribute_definitions.end(), attr_def) != _attribute_definitions.end();
    }
    for (const auto& attr : rhs.AttributeDefaults())
    {
        equal &= std::find(_attribute_defaults.begin(), _attribute_defaults.end(), attr) != _attribute_defaults.end();
    }
    for (const auto& attr : rhs.AttributeValues())
    {
        equal &= std::find(_attribute_values.begin(), _attribute_values.end(), attr) != _attribute_values.end();
    }
    equal &= _comment == rhs.Comment();
    return equal;
}
bool NetworkImpl::operator!=(const INetwork& rhs) const
{
    return !(*this == rhs);
}

std::map<std::string, std::unique_ptr<INetwork>> INetwork::LoadNetworkFromFile(const std::filesystem::path& filename)
{
    auto result = std::map<std::string, std::unique_ptr<INetwork>>();
    auto is = std::ifstream(filename);
    if (!is.is_open())
    {
        std::cout << "Error: Could not open file " << filename << "\n";
    }
    else if (filename.extension() == ".dbc")
    {
        auto net = LoadDBCFromIs(is);
        if (net)
        {
            result.insert(std::make_pair("", std::move(net)));
        }
    }
#ifdef ENABLE_KCD
    else if (filename.extension() == ".kcd") {
        result = LoadKCDFromIs(is);
    }
#endif
    return std::move(result);
}
