
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
		static std::unique_ptr<Network> create();
		static std::unique_ptr<const Network> fromDBC(std::istream& is);

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

		virtual Message* addMessage(uint64_t id) = 0;

		void serializeToStream(std::ostream& os) const;
	};
}

DBCPPP_API bool operator>>(std::istream& is, dbcppp::Network& net);
