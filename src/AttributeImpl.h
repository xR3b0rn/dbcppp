
#pragma once

#include <iostream>
#include "dbcppp/Attribute.h"

namespace dbcppp
{
    class AttributeImpl final
        : public IAttribute
    {
    public:
        AttributeImpl(std::string&& name, IAttributeDefinition::EObjectType object_type, IAttribute::value_t value);

        virtual std::unique_ptr<IAttribute> Clone() const override;

        virtual const std::string& Name() const override;
        virtual IAttributeDefinition::EObjectType ObjectType() const override;
        virtual const value_t& Value() const override;
        
        virtual bool operator==(const IAttribute& rhs) const override;
        virtual bool operator!=(const IAttribute& rhs) const override;

    private:
        std::string _name;
        IAttributeDefinition::EObjectType _object_type;
        IAttribute::value_t _value;
    };
}