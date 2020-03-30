
#pragma once

#include <iostream>
#include "../../include/dbcppp/Attribute.h"

namespace dbcppp
{
    class AttributeImpl final
        : public Attribute
    {
    public:
        AttributeImpl(std::string&& name, AttributeDefinition::ObjectType object_type, Attribute::value_t value);

        virtual std::unique_ptr<Attribute> clone() const override;

        virtual const std::string& getName() const override;
        virtual AttributeDefinition::ObjectType getObjectType() const override;
        virtual const value_t& getValue() const override;

    private:
        std::string _name;
        AttributeDefinition::ObjectType _object_type;
        Attribute::value_t _value;
    };
}
