#pragma once

#include <map>
#include <string>
#include <memory>
#include <functional>

#include "Export.h"
#include "Iterator.h"
#include "Attribute.h"

namespace dbcppp
{
    class DBCPPP_API INode
    {
    public:
        static std::unique_ptr<INode> Create(
            std::string&& name,
            std::string&& comment,
            std::vector<std::unique_ptr<IAttribute>>&& attribute_values);
            
        virtual std::unique_ptr<INode> Clone() const = 0;

        virtual ~INode() = default;
        virtual const std::string& Name() const = 0;
        virtual const IAttribute& AttributeValues_Get(std::size_t i) const = 0;
        virtual uint64_t AttributeValues_Size() const = 0;
        virtual const std::string& Comment() const = 0;

        DBCPPP_MAKE_ITERABLE(INode, AttributeValues, IAttribute);
        
        virtual bool operator==(const dbcppp::INode& rhs) const = 0;
        virtual bool operator!=(const dbcppp::INode& rhs) const = 0;
    };
}