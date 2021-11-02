#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include <cstddef>
#include <functional>

#include "Export.h"
#include "Iterator.h"
#include "Node.h"
#include "Attribute.h"
#include "ValueEncodingDescription.h"

namespace dbcppp
{
    class DBCPPP_API IEnvironmentVariable
    {
    public:
        enum class EVarType
        {
            Integer, Float, String, Data
        };
        enum class EAccessType
        {
            Unrestricted    = 0x0000,
            Read            = 0x0001,
            Write           = 0x0002,
            ReadWrite       = 0x0003,
            Unrestricted_   = 0x8000,
            Read_           = 0x8001,
            Write_          = 0x8002,
            ReadWrite_      = 0x8003
        };

        static std::unique_ptr<IEnvironmentVariable> Create(
              std::string&& name
            , EVarType var_type
            , double minimum
            , double maximum
            , std::string&& unit
            , double initial_value
            , uint64_t ev_id
            , EAccessType access_type
            , std::vector<std::string>&& access_nodes
            , std::vector<std::unique_ptr<IValueEncodingDescription>>&& value_encoding_descriptions
            , uint64_t data_size
            , std::vector<std::unique_ptr<IAttribute>>&& attribute_values
            , std::string&& comment);
            
        virtual std::unique_ptr<IEnvironmentVariable> Clone() const = 0;

        virtual ~IEnvironmentVariable() = default;
        virtual const std::string& Name() const = 0;
        virtual EVarType VarType() const = 0;
        virtual double Minimum() const = 0;
        virtual double Maximum() const = 0;
        virtual const std::string& Unit() const = 0;
        virtual double InitialValue() const = 0;
        virtual uint64_t EvId() const = 0;
        virtual EAccessType AccessType() const = 0;
        virtual const std::string& AccessNodes_Get(std::size_t i) const = 0;
        virtual uint64_t AccessNodes_Size() const = 0;
        virtual const IValueEncodingDescription& ValueEncodingDescriptions_Get(std::size_t i) const = 0;
        virtual uint64_t ValueEncodingDescriptions_Size() const = 0;
        virtual uint64_t DataSize() const = 0;
        virtual const IAttribute& AttributeValues_Get(std::size_t i) const = 0;
        virtual uint64_t AttributeValues_Size() const = 0;
        virtual const std::string& Comment() const = 0;
        
        DBCPPP_MAKE_ITERABLE(IEnvironmentVariable, AccessNodes, std::string);
        DBCPPP_MAKE_ITERABLE(IEnvironmentVariable, ValueEncodingDescriptions, IValueEncodingDescription);
        DBCPPP_MAKE_ITERABLE(IEnvironmentVariable, AttributeValues, IAttribute);

        virtual bool operator==(const IEnvironmentVariable& rhs) const = 0;
        virtual bool operator!=(const IEnvironmentVariable& rhs) const = 0;
    };
}