
#pragma once

#include <map>
#include <set>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "Export.h"
#include "BitTiming.h"
#include "ValueTable.h"
#include "Node.h"
#include "Message.h"
#include "EnvironmentVariable.h"
#include "SignalType.h"
#include "AttributeDefinition.h"
#include "Attribute.h"
#include "SignalExtendedValueType.h"

namespace dbcppp
{
	class DBCPPP_API Network
	{
	public:
		using new_symbols_t = std::vector<std::string>;
		using nodes_t = std::map<std::string, Node>;
		using value_tables_t = std::map<std::string, ValueTable>;
		using messages_t = std::unordered_map<uint64_t, Message>;
		using environment_variable_t = std::map<std::string, EnvironmentVariable>;
		using signal_types_t = std::map<std::string, SignalType>;
		using attribute_definitions_t = std::map<std::string, AttributeDefinition>;
		using attribute_defaults_t = std::map<std::string, Attribute>;
		using attribute_values_t = std::map<std::string, Attribute>;
		using signal_extended_value_types_t = std::vector<SignalExtendedValueType>;

		static std::unique_ptr<Network> create();
		virtual ~Network() = default;
		virtual const std::string& getVersion() const = 0;
		virtual bool hasNewSymbol(const std::string& name) const = 0;
		virtual std::vector<const std::string*> getNewSymbols() const = 0;
		virtual const BitTiming& getBitTiming() const = 0;
		virtual const Node* getNodeByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const Node*>> getNodes() const = 0;
		virtual const ValueTable* getValueTableByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const ValueTable*>> getValueTables() const = 0;
		virtual const Message* getMessageById(uint64_t id) const = 0;
		virtual std::vector<std::pair<uint64_t, const Message*>> getMessages() const = 0;
		virtual const EnvironmentVariable* getEnvironmentVariableByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const EnvironmentVariable*>> getEnvironmentVariables() const = 0;
		virtual const SignalType* getSignalTypeByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const SignalType*>> getSignalTypes() const = 0;
		virtual const AttributeDefinition* getAttributeDefinitionByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const AttributeDefinition*>> getAttributeDefinitions() const = 0;
		virtual const Attribute* getAttributeDefaultByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeDefaults() const = 0;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const = 0;
		virtual std::vector<const SignalExtendedValueType*> getSignalExtendedValues() const = 0;
		virtual const std::string& getComment() const = 0;
	};
}

DBCPPP_API bool operator>>(std::istream& is, dbcppp::Network& net);
