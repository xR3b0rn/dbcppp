#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <functional>
#include <variant>
#include <memory>

#include "Export.h"
#include "Iterator.h"

namespace dbcppp
{
    class DBCPPP_API IAttributeDefinition
    {
    public:
        enum class EObjectType
        {
            Network,
            Node,
            Message,
            Signal,
            EnvironmentVariable,
        };
        struct ValueTypeInt
        {
            int64_t minimum;
            int64_t maximum;
        };
        struct ValueTypeHex
        {
            int64_t minimum;
            int64_t maximum;
        };
        struct ValueTypeFloat
        {
            double minimum;
            double maximum;
        };
        struct ValueTypeString
        {
        };
        struct ValueTypeEnum
        {
            std::vector<std::string> values;
        };
        using value_type_t = std::variant<ValueTypeInt, ValueTypeHex, ValueTypeFloat, ValueTypeString, ValueTypeEnum>;
        
        static std::unique_ptr<IAttributeDefinition> Create(
              std::string&& name
            , EObjectType object_type
            , value_type_t&& value_type);
            
        virtual std::unique_ptr<IAttributeDefinition> Clone() const = 0;

        virtual ~IAttributeDefinition() = default;
        virtual EObjectType ObjectType() const = 0;
        virtual const std::string& Name() const = 0;
        virtual const value_type_t& ValueType() const = 0;
        
        virtual bool operator==(const IAttributeDefinition& rhs) const = 0;
        virtual bool operator!=(const IAttributeDefinition& rhs) const = 0;
    };
}