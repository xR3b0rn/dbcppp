
#pragma once

#include "../../include/dbcppp/Network.h"
#include "BitTimingImpl.h"
#include "ValueTableImpl.h"
#include "MessageImpl.h"
#include "EnvironmentVariableImpl.h"
#include "SignalTypeImpl.h"
#include "AttributeDefinitionImpl.h"
#include "AttributeImpl.h"

namespace dbcppp
{
	class NetworkImpl final
		: public Network
	{
	public:
		NetworkImpl(
			  std::string&& version
			, std::set<std::string>&& new_symbols
			, BitTimingImpl&& bit_timing
			, std::map<std::string, NodeImpl>&& nodes
			, std::map<std::string, ValueTableImpl>&& value_tables
			, std::unordered_map<uint64_t, MessageImpl>&& messages
			, std::map<std::string, EnvironmentVariableImpl>&& environment_variables
			, std::map<std::string, AttributeDefinitionImpl>&& attribute_definitions
			, std::map<std::string, AttributeImpl>&& attribute_defaults
			, std::map<std::string, AttributeImpl>&& attribute_values
			, std::string&& comment);
		NetworkImpl(NetworkImpl&&) = default;
		NetworkImpl& operator=(NetworkImpl&&) = default;

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
		virtual const AttributeDefinition* getAttributeDefinitionByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const AttributeDefinition*>> getAttributeDefinitions() const override;
		virtual const Attribute* getAttributeDefaultByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeDefaults() const override;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const override;
		virtual const std::string& getComment() const override;

		virtual const Message* findParentMessage(const Signal* sig) const override;
		
		std::string& version();
		std::set<std::string>& newSymbols();
		BitTimingImpl& bitTiming();
		std::map<std::string, NodeImpl>& nodes();
		std::map<std::string, ValueTableImpl>& valueTables();
		std::unordered_map<uint64_t, MessageImpl>& messages();
		std::map<std::string, EnvironmentVariableImpl>& environmentVariables();
		std::map<std::string, AttributeDefinitionImpl>& attributeDefinitions();
		std::map<std::string, AttributeImpl>& attributeDefaults();
		std::map<std::string, AttributeImpl>& attributeValues();
		std::string& comment();

	private:
		std::string _version;
		std::set<std::string> _new_symbols;
		BitTimingImpl _bit_timing;
		std::map<std::string, NodeImpl> _nodes;
		std::map<std::string, ValueTableImpl> _value_tables;
		std::unordered_map<uint64_t, MessageImpl> _messages;
		std::map<std::string, EnvironmentVariableImpl> _environment_variables;
		std::map<std::string, AttributeDefinitionImpl> _attribute_definitions;
		std::map<std::string, AttributeImpl> _attribute_defaults;
		std::map<std::string, AttributeImpl> _attribute_values;
		std::string _comment;
	};
}
