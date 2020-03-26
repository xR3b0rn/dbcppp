
#pragma once

#include <string>
#include <cstddef>
#include <boost/variant.hpp>

#include "Export.h"
#include "AttributeDefinition.h"

namespace dbcppp
{
    class Network;
    class DBCPPP_API Attribute
    {
    public:
        using hex_value_t = int64_t;
        using value_t = boost::variant<int64_t, double, std::string>;

        static std::unique_ptr<Attribute> create(
              std::string&& name
            , AttributeDefinition::ObjectType object_type
            , value_t value);
            
        virtual std::unique_ptr<Attribute> clone() const = 0;

        virtual ~Attribute() = default;
        virtual const std::string& getName() const = 0;
        virtual AttributeDefinition::ObjectType getObjectType() const = 0;
        virtual const value_t& getValue() const = 0;
        
        void serializeToStream(std::ostream& os, const Network& net) const;
    };
}
