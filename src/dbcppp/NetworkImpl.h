
#pragma once

#include "Network.h"
#include "BitTimingImpl.h"
#include "ValueTableImpl.h"
#include "MessageImpl.h"
#include "EnvironmentVariableImpl.h"
#include "SignalTypeImpl.h"
#include "AttributeDefinitionImpl.h"
#include "AttributeImpl.h"

namespace dbcppp
{
	class NetworkImpl
		: public Network
	{
	public:
		virtual const std::string& getVersion() const override;
		virtual bool hasNewSymbol(const std::string& name) const override;
		virtual std::vector<const std::string*> getNewSymbols() const override;
		virtual const BitTiming& getBitTiming() const override;
		virtual const Node* getNodeByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const Node*>> getNodes() const override;
		virtual const ValueTable* getValueTableByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const ValueTable*>> getValueTables() const override;
		virtual const Message* getMessageById(uint64_t id) const override;
		virtual std::vector<std::pair<uint64_t, const Message*>> getMessages() const override;
		virtual const EnvironmentVariable* getEnvironmentVariableByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const EnvironmentVariable*>> getEnvironmentVariables() const override;
		virtual const SignalType* getSignalTypeByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const SignalType*>> getSignalTypes() const override;
		virtual const AttributeDefinition* getAttributeDefinitionByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const AttributeDefinition*>> getAttributeDefinitions() const override;
		virtual const Attribute* getAttributeDefaultByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeDefaults() const override;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const override;
		virtual const std::string& getComment() const override;

		virtual const Message* findParentMessage(const Signal* sig) const override;

		std::string _version;
		std::vector<std::string> _new_symbols;
		BitTimingImpl _bit_timing;
		std::map<std::string, NodeImpl> _nodes;
		std::map<std::string, ValueTableImpl> _value_tables;
		std::unordered_map<uint64_t, MessageImpl> _messages;
		std::map<std::string, EnvironmentVariableImpl> _environment_variables;
		std::map<std::string, SignalTypeImpl> _signal_types;
		std::map<std::string, AttributeDefinitionImpl> _attribute_definitions;
		std::map<std::string, AttributeImpl> _attribute_defaults;
		std::map<std::string, AttributeImpl> _attribute_values;
		//std::map<std::string, AttributeRelation> attribute_relation_values;
		std::string _comment;
	};
}
