#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <istream>
#include <functional>
#include <unordered_map>
#include <filesystem>

#include "Export.h"
#include "Iterator.h"
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
    class DBCPPP_API INetwork
    {
    public:
        static std::unique_ptr<INetwork> Create(
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
            , std::string&& comment);
        static std::map<std::string, std::unique_ptr<INetwork>> LoadNetworkFromFile(const std::filesystem::path& filename);
        static std::unique_ptr<INetwork> LoadDBCFromIs(std::istream& is);
        static std::map<std::string, std::unique_ptr<INetwork>> LoadKCDFromIs(std::istream& is);
        
        virtual std::unique_ptr<INetwork> Clone() const = 0;

        virtual ~INetwork() = default;
        virtual const std::string& Version() const = 0;
        virtual const std::string& NewSymbols_Get(std::size_t i) const = 0;
        virtual uint64_t NewSymbols_Size() const = 0;
        virtual const IBitTiming& BitTiming() const = 0;
        virtual const INode& Nodes_Get(std::size_t i) const = 0;
        virtual uint64_t Nodes_Size() const = 0;
        virtual const IValueTable& ValueTables_Get(std::size_t i) const = 0;
        virtual uint64_t ValueTables_Size() const = 0;
        virtual const IMessage& Messages_Get(std::size_t i) const = 0;
        virtual uint64_t Messages_Size() const = 0;
        virtual const IEnvironmentVariable& EnvironmentVariables_Get(std::size_t i) const = 0;
        virtual uint64_t EnvironmentVariables_Size() const = 0;
        virtual const IAttributeDefinition& AttributeDefinitions_Get(std::size_t i) const = 0;
        virtual uint64_t AttributeDefinitions_Size() const = 0;
        virtual const IAttribute& AttributeDefaults_Get(std::size_t i) const = 0;
        virtual uint64_t AttributeDefaults_Size() const = 0;
        virtual const IAttribute& AttributeValues_Get(std::size_t i) const = 0;
        virtual uint64_t AttributeValues_Size() const = 0;
        virtual const std::string& Comment() const = 0;
        
        DBCPPP_MAKE_ITERABLE(INetwork, NewSymbols, std::string);
        DBCPPP_MAKE_ITERABLE(INetwork, Nodes, INode);
        DBCPPP_MAKE_ITERABLE(INetwork, ValueTables, IValueTable);
        DBCPPP_MAKE_ITERABLE(INetwork, Messages, IMessage);
        DBCPPP_MAKE_ITERABLE(INetwork, EnvironmentVariables, IEnvironmentVariable);
        DBCPPP_MAKE_ITERABLE(INetwork, AttributeDefinitions, IAttributeDefinition);
        DBCPPP_MAKE_ITERABLE(INetwork, AttributeDefaults, IAttribute);
        DBCPPP_MAKE_ITERABLE(INetwork, AttributeValues, IAttribute);

        virtual const IMessage* ParentMessage(const ISignal* sig) const = 0;

        virtual bool operator==(const INetwork& rhs) const = 0;
        virtual bool operator!=(const INetwork& rhs) const = 0;

        void Merge(std::unique_ptr<INetwork>&& other);
    };
}