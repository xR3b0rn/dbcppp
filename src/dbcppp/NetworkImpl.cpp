
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
const std::string& NetworkImpl::getComment() const
{
	return _comment;
}
const Message* NetworkImpl::findParentMessage(const Signal* sig) const
{
	const Message* result = nullptr;
	for (const auto& p : _messages)
	{
		const MessageImpl& msg = p.second;
		auto iter = msg._signals.find(sig->getName());
		if (iter != msg._signals.end() && &iter->second == sig)
		{
			result = &msg;
			break;
		}
	}
	return result;
}

void Network::serializeToStream(std::ostream& os, const Network& net) const
{
	os << "VERSION \"";
	if (getVersion() != "")
	{
		os << getVersion();
	}
	os << "\"";
	os << "\n";
	os << "NS_:";
	for (const auto& ns : getNewSymbols())
	{
		os << "\n " << *ns;
	}
	os << "\n";
	getBitTiming().serializeToStream(os, net);
	os << "\n";
	os << "BU_:";
	for (const auto& n : getNodes())
	{
		os << " " << n.second->getName(); 
	}
	for (const auto& vt : getValueTables())
	{
		os << "\n";
		vt.second->serializeToStream(os, net);
	}
	for (const auto& m : getMessages())
	{
		os << "\n";
		m.second->serializeToStream(os, net);
	}
	// serialize message_transmitters
	for (const auto& m : getMessages())
	{
		auto transmitters = m.second->getMessageTransmitters();
		if (transmitters.size())
		{
			os << "\n";
			os << "BO_TX_BU_ " << m.second->getId() << " :";
			for (const auto& t : transmitters)
			{
				os << " " << t;
			}
		}
	}
	for (const auto& ev : getEnvironmentVariables())
	{
		os << "\n";
		ev.second->serializeToStream(os, net);
	}
	for (const auto& ev : getEnvironmentVariables())
	{
		if (ev.second->getVarType() == EnvironmentVariable::VarType::Data)
		{
			os << "\n";
			os << "ENVVAR_DATA_ " << ev.second->getName() << " : " << ev.second->getDataSize() << ";";
		}
	}
	for (const auto& st : getSignalTypes())
	{
		os << "\n";
		st.second->serializeToStream(os, net);
	}
	// serialize comments
	// Network comment
	if (net.getComment() != "")
	{
		os << "\n";
		os << "CM_ " << net.getComment() << ";";
	}
	// Node comments
	for (const auto& n : net.getNodes())
	{
		if (n.second->getComment() != "")
		{
			os << "\n";
			os << "CM_ BU_ " << n.second->getName() << " \"" << n.second->getComment() << "\"" << ";";
		}
	}
	// Message comments
	for (const auto& m : net.getMessages())
	{
		if (m.second->getComment() != "")
		{
			os << "\n";
			os << "CM_ BO_ " << m.second->getId() << " \"" << m.second->getComment() << "\"" << ";";
		}
	}
	// Signal comments
	for (const auto& m : net.getMessages())
	{
		for (const auto& s : m.second->getSignals())
		{
			if (s.second->getComment() != "")
			{
				os << "\n";
				os << "CM_ SG_ " << m.second->getId() << " " << s.second->getName() << " \"" << s.second->getComment() << "\"" << ";";
			}
		}
	}
	// EnvironmentVariable comments
	for (const auto& ev : net.getEnvironmentVariables())
	{
		if (ev.second->getComment() != "")
		{
			os << "\n";
			os << "CM_ EV_ " << ev.second->getName() << " \"" << ev.second->getComment() << "\"" << ";";
		}
	}
	for (const auto& ad : net.getAttributeDefinitions())
	{
		os << "\n";
		ad.second->serializeToStream(os, net);
	}
	for (const auto& ad : net.getAttributeDefaults())
	{
		os << "\n";
		ad.second->serializeToStream(os, net);
	}
	// Serialize Attribute Values
	for (const auto& val : net.getAttributeValues())
	{
		os << "\n";
		val.second->serializeToStream(os, net);
	}
	for (const auto& n : net.getNodes())
	{
		for (const auto& val : n.second->getAttributeValues())
		{
			os << "\n";
			val.second->serializeToStream(os, net);
		}
	}
	for (const auto& m : net.getMessages())
	{
		for (const auto& val : m.second->getAttributeValues())
		{
			os << "\n";
			val.second->serializeToStream(os, net);
		}
	}
	for (const auto& m : net.getMessages())
	{
		for (const auto& s : m.second->getSignals())
		{
			for (const auto& val : s.second->getAttributeValues())
			{
				os << "\n";
				val.second->serializeToStream(os, net);
			}
		}
	}
	for (const auto& ev : net.getEnvironmentVariables())
	{
		for (const auto& val : ev.second->getAttributeValues())
		{
			os << "\n";
			val.second->serializeToStream(os, net);
		}
	}
	// Serialize value descriptions
	for (const auto& m : net.getMessages())
	{
		for (const auto& s : m.second->getSignals())
		{
			auto vds = s.second->getValueDescriptions();
			if (vds.size())
			{
				os << "\n";
				os << "VAL_ " << m.second->getId() << " " << s.second->getName();
				for (const auto& vd : vds)
				{
					os << " " << vd.first << " \"" << *vd.second << "\"";
				}
				os << ";";
			}
		}
	}
	for (const auto& m : net.getMessages())
	{
		for (const auto& s : m.second->getSignals())
		{
			if (s.second->getExtendedValueType())
			{
				uint64_t type = 0;
				switch (*s.second->getExtendedValueType())
				{
				case Signal::ExtendedValueType::Integer: type = 0; break;
				case Signal::ExtendedValueType::Float: type = 1; break;
				case Signal::ExtendedValueType::Double: type = 2; break;
				}
				os << "\n";
				os << "SIG_VALTYPE_ " << m.second->getId() << " " << s.second->getName() << type << ";";
			}
		}
	}
}
