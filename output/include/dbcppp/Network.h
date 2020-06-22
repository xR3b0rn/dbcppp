#pragma once

#include <map>
#include <set>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <istream>
#include <functional>
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
            , std::set<std::string>&& new_symbols
            , std::unique_ptr<BitTiming>&& bit_timing
            , std::map<std::string, std::unique_ptr<Node>>&& nodes
            , std::map<std::string, std::unique_ptr<ValueTable>>&& value_tables
            , std::unordered_map<uint64_t, std::unique_ptr<Message>>&& messages
            , std::map<std::string, std::unique_ptr<EnvironmentVariable>>&& environment_variables
            , std::map<std::string, std::unique_ptr<AttributeDefinition>>&& attribute_definitions
            , std::map<std::string, std::unique_ptr<Attribute>>&& attribute_defaults
            , std::map<std::string, std::unique_ptr<Attribute>>&& attribute_values
            , std::string&& comment);
        static std::unique_ptr<Network> fromDBC(std::istream& is);
        static std::unique_ptr<Network> fromDBC(std::istream& is, std::unique_ptr<Network> network);
        static std::unique_ptr<Network> fromKCD(const std::string& filename);
        
        virtual std::unique_ptr<Network> clone() const = 0;

        virtual ~Network() = default;
        virtual const std::string& getVersion() const = 0;
        virtual bool hasNewSymbol(const std::string& name) const = 0;
        virtual void forEachNewSymbol(std::function<void(const std::string&)> cb) const = 0;
        virtual const BitTiming& getBitTiming() const = 0;
        virtual const Node* getNodeByName(const std::string& name) const = 0;
        virtual const Node* findNode(std::function<bool(const Node&)>&& pred) const = 0;
        virtual void forEachNode(std::function<void(const Node&)>&& cb) const= 0;
        virtual const ValueTable* getValueTableByName(const std::string& name) const = 0;
        virtual const ValueTable* findValueTable(std::function<bool(const ValueTable&)>&& pred) const = 0;
        virtual void forEachValueTable(std::function<void(const ValueTable&)>&& cb) const = 0;
        virtual const Message* getMessageById(uint64_t id) const = 0;
        virtual const Message* findMessage(std::function<bool(const Message&)>&& pred) const = 0;
        virtual void forEachMessage(std::function<void(const Message&)>&& cb) const = 0;
        virtual const EnvironmentVariable* getEnvironmentVariableByName(const std::string& name) const = 0;
        virtual const EnvironmentVariable* findEnvironmentVariable(std::function<bool(const EnvironmentVariable&)>&& cb) const = 0;
        virtual void forEachEnvironmentVariable(std::function<void(const EnvironmentVariable&)>&& cb) const = 0;
        virtual const AttributeDefinition* getAttributeDefinitionByName(const std::string& name) const = 0;
        virtual const AttributeDefinition* findAttributeDefinition(std::function<bool(const AttributeDefinition&)>&& pred) const = 0;
        virtual void forEachAttributeDefinition(std::function<void(const AttributeDefinition&)>&& cb) const = 0;
        virtual const Attribute* getAttributeDefaultByName(const std::string& name) const = 0;
        virtual const Attribute* findAttributeDefault(std::function<bool(const Attribute&)>&& pred) const = 0;
        virtual void forEachAttributeDefault(std::function<void(const Attribute&)>&& cb) const = 0;
        virtual const Attribute* getAttributeValueByName(const std::string& name) const = 0;
        virtual const Attribute* findAttributeValue(std::function<bool(const Attribute&)>&& pred) const = 0;
        virtual void forEachAttributeValue(std::function<void(const Attribute&)>&& cb) const = 0;
        virtual const std::string& getComment() const = 0;
        
        virtual const Message* findParentMessage(const Signal* sig) const = 0;

        void merge(std::unique_ptr<Network>&& other);
    };
}
