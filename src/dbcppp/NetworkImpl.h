
#pragma once

#include <robin-map/tsl/robin_map.h>
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
            , tsl::robin_map<uint64_t, MessageImpl>&& messages
            , std::map<std::string, EnvironmentVariableImpl>&& environment_variables
            , std::map<std::string, AttributeDefinitionImpl>&& attribute_definitions
            , std::map<std::string, AttributeImpl>&& attribute_defaults
            , std::map<std::string, AttributeImpl>&& attribute_values
            , std::string&& comment);
            
        virtual std::unique_ptr<Network> clone() const override;

        virtual const std::string& getVersion() const override;
        virtual bool hasNewSymbol(const std::string& name) const override;
        virtual void forEachNewSymbol(std::function<void(const std::string&)> cb) const override;
        virtual const BitTiming& getBitTiming() const override;
        virtual const Node* getNodeByName(const std::string& name) const override;
        virtual const Node* findNode(std::function<bool(const Node&)>&& pred) const override;
        virtual void forEachNode(std::function<void(const Node&)>&& cb) const override;
        virtual const ValueTable* getValueTableByName(const std::string& name) const override;
        virtual const ValueTable* findValueTable(std::function<bool(const ValueTable&)>&& pred) const override;
        virtual void forEachValueTable(std::function<void(const ValueTable&)>&& cb) const override;
        virtual const Message* getMessageById(uint64_t id) const override;
        virtual const Message* findMessage(std::function<bool(const Message&)>&& pred) const override;
        virtual void forEachMessage(std::function<void(const Message&)>&& cb) const override;
        virtual const EnvironmentVariable* getEnvironmentVariableByName(const std::string& name) const override;
        virtual const EnvironmentVariable* findEnvironmentVariable(std::function<bool(const EnvironmentVariable&)>&& pred) const override;
        virtual void forEachEnvironmentVariable(std::function<void(const EnvironmentVariable&)>&& cb) const override;
        virtual const AttributeDefinition* getAttributeDefinitionByName(const std::string& name) const override;
        virtual const AttributeDefinition* findAttributeDefinition(std::function<bool(const AttributeDefinition&)>&& pred) const override;
        virtual void forEachAttributeDefinition(std::function<void(const AttributeDefinition&)>&& cb) const override;
        virtual const Attribute* getAttributeDefaultByName(const std::string& name) const override;
        virtual const Attribute* findAttributeDefault(std::function<bool(const Attribute&)>&& pred) const override;
        virtual void forEachAttributeDefault(std::function<void(const Attribute&)>&& cb) const override;
        virtual const Attribute* getAttributeValueByName(const std::string& name) const override;
        virtual const Attribute* findAttributeValue(std::function<bool(const Attribute&)>&& pred) const override;
        virtual void forEachAttributeValue(std::function<void(const Attribute&)>&& cb) const override;
        virtual const std::string& getComment() const override;

        virtual const Message* findParentMessage(const Signal* sig) const override;

        std::string& version();
        std::set<std::string>& newSymbols();
        BitTimingImpl& bitTiming();
        std::map<std::string, NodeImpl>& nodes();
        std::map<std::string, ValueTableImpl>& valueTables();
        tsl::robin_map<uint64_t, MessageImpl>& messages();
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
        tsl::robin_map<uint64_t, MessageImpl> _messages;
        std::map<std::string, EnvironmentVariableImpl> _environment_variables;
        std::map<std::string, AttributeDefinitionImpl> _attribute_definitions;
        std::map<std::string, AttributeImpl> _attribute_defaults;
        std::map<std::string, AttributeImpl> _attribute_values;
        std::string _comment;
    };
}
