
#include "NetworkImpl.h"

using namespace dbcppp;

const std::string& NetworkImpl::getVersion() const
{
	return _version;
}
bool NetworkImpl::hasNewSymbol(const std::string& name) const
{
	return std::find(_new_symbols.begin(), _new_symbols.end(), name) != _new_symbols.end();
}
std::vector<const std::string*> NetworkImpl::getNewSymbols() const
{
	std::vector<const std::string*> result;
	for (auto& ns : _new_symbols)
	{
		result.emplace_back(&ns);
	}
	return result;
}
const BitTiming& NetworkImpl::getBitTiming() const
{
	return _bit_timing;
}
const Node* NetworkImpl::getNodeByName(const std::string& name) const
{
	const Node* result = nullptr;
	auto iter = _nodes.find(name);
	if (iter != _nodes.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const Node*>> NetworkImpl::getNodes() const
{
	std::vector<std::pair<std::string, const Node*>> result;
	for (auto& n : _nodes)
	{
		result.emplace_back(n.first, &n.second);
	}
	return result;
}
const ValueTable* NetworkImpl::getValueTableByName(const std::string& name) const
{
	const ValueTable* result = nullptr;
	auto iter = _value_tables.find(name);
	if (iter != _value_tables.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const ValueTable*>> NetworkImpl::getValueTables() const
{
	std::vector<std::pair<std::string, const ValueTable*>> result;
	for (auto& vt : _value_tables)
	{
		result.emplace_back(vt.first, &vt.second);
	}
	return result;
}
const Message* NetworkImpl::getMessageById(uint64_t id) const
{
	const Message* result = nullptr;
	auto iter = _messages.find(id);
	if (iter != _messages.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<uint64_t, const Message*>> NetworkImpl::getMessages() const
{
	std::vector<std::pair<uint64_t, const Message*>> result;
	for (auto& m : _messages)
	{
		result.emplace_back(m.first, &m.second);
	}
	return result;
}
const EnvironmentVariable* NetworkImpl::getEnvironmentVariableByName(const std::string& name) const
{
	const EnvironmentVariable* result = nullptr;
	auto iter = _environment_variables.find(name);
	if (iter != _environment_variables.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const EnvironmentVariable*>> NetworkImpl::getEnvironmentVariables() const
{
	std::vector<std::pair<std::string, const EnvironmentVariable*>> result;
	for (auto& ev : _environment_variables)
	{
		result.emplace_back(ev.first, &ev.second);
	}
	return result;
}
const SignalType* NetworkImpl::getSignalTypeByName(const std::string& name) const
{
	const SignalType* result = nullptr;
	auto iter = _signal_types.find(name);
	if (iter != _signal_types.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const SignalType*>> NetworkImpl::getSignalTypes() const
{
	std::vector<std::pair<std::string, const SignalType*>> result;
	for (auto& st : _signal_types)
	{
		result.emplace_back(st.first, &st.second);
	}
	return result;
}
const AttributeDefinition* NetworkImpl::getAttributeDefinitionByName(const std::string& name) const
{
	const AttributeDefinition* result = nullptr;
	auto iter = _attribute_definitions.find(name);
	if (iter != _attribute_definitions.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const AttributeDefinition*>> NetworkImpl::getAttributeDefinitions() const
{
	std::vector<std::pair<std::string, const AttributeDefinition*>> result;
	for (auto& ad : _attribute_definitions)
	{
		result.emplace_back(ad.first, &ad.second);
	}
	return result;
}
const Attribute* NetworkImpl::getAttributeDefaultByName(const std::string& name) const
{
	const Attribute* result = nullptr;
	auto iter = _attribute_defaults.find(name);
	if (iter != _attribute_defaults.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const Attribute*>> NetworkImpl::getAttributeDefaults() const
{
	std::vector<std::pair<std::string, const Attribute*>> result;
	for (auto& ad : _attribute_defaults)
	{
		result.emplace_back(ad.first, &ad.second);
	}
	return result;
}
const Attribute* NetworkImpl::getAttributeValueByName(const std::string& name) const
{
	const Attribute* result = nullptr;
	auto iter = _attribute_values.find(name);
	if (iter != _attribute_values.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const Attribute*>> NetworkImpl::getAttributeValues() const
{
	std::vector<std::pair<std::string, const Attribute*>> result;
	for (auto& av : _attribute_values)
	{
		result.emplace_back(av.first, &av.second);
	}
	return result;
}

std::vector<const SignalExtendedValueType*> NetworkImpl::getSignalExtendedValues() const
{
    std::vector<const SignalExtendedValueType*> result;
    for (auto& sigval : _signal_extended_value_types)
    {
       result.emplace_back(&sigval);
    }
    return result;
}

const std::string& NetworkImpl::getComment() const
{
	return _comment;
}

