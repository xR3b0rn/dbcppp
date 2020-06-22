#pragma once

#include <map>
#include <string>
#include <memory>
#include <functional>

#include "Export.h"
#include "Attribute.h"

namespace dbcppp
{
    class DBCPPP_API Node
    {
    public:
        static std::unique_ptr<Node> create(
            std::string&& name,
            std::string&& comment,
            std::map<std::string, std::unique_ptr<Attribute>>&& attribute_values);
            
        virtual std::unique_ptr<Node> clone() const = 0;

        virtual ~Node() = default;
        virtual const std::string& getName() const = 0;
        virtual const Attribute* getAttributeValueByName(const std::string& name) const = 0;
        virtual const Attribute* findAttributeValue(std::function<bool(const Attribute&)>&& pred) const = 0;
        virtual void forEachAttributeValue(std::function<void(const Attribute&)>&& cb) const = 0;
        virtual const std::string& getComment() const = 0;
    };
}