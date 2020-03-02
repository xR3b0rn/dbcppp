
#pragma once

#include <map>
#include <set>
#include <memory>
#include <string>
#include <vector>
#include <istream>
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

namespace dbcppp
{
	class DBCPPP_API Network
	{
	public:
		static std::unique_ptr<Network> create(
			  std::string&& version
			, std::vector<std::string>&& new_symbols
			, std::unique_ptr<BitTiming>&& bit_timing
			, std::map<std::string, std::unique_ptr<Node>>&& nodes
			, std::map<std::string, std::unique_ptr<ValueTable>>&& value_tables
			, std::unordered_map<uint64_t, std::unique_ptr<Message>>&& messages
			, std::map<std::string, std::unique_ptr<EnvironmentVariable>>&& environment_variables
			, std::map<std::string, std::unique_ptr<AttributeDefinition>>&& attribute_definitions
			, std::map<std::string, std::unique_ptr<Attribute>>&& attribute_defaults
			, std::map<std::string, std::unique_ptr<Attribute>>&& attribute_values
			, std::string&& comment);
		static std::unique_ptr<const Network> fromDBCIStream(std::istream& is);

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
		virtual const AttributeDefinition* getAttributeDefinitionByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const AttributeDefinition*>> getAttributeDefinitions() const = 0;
		virtual const Attribute* getAttributeDefaultByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeDefaults() const = 0;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const = 0;
		virtual const std::string& getComment() const = 0;
		
		virtual const Message* findParentMessage(const Signal* sig) const = 0;

		void serializeToStream(std::ostream& os) const;
	};
}
