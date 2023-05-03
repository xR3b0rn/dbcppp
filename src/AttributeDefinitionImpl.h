#pragma once

#include "dbcppp/AttributeDefinition.h"

namespace dbcppp
{
    class AttributeDefinitionImpl final
        : public IAttributeDefinition
    {
    public:
        AttributeDefinitionImpl(std::string&& name, EObjectType object_type, value_type_t value_type);
        
        virtual std::unique_ptr<IAttributeDefinition> Clone() const override;

        virtual EObjectType ObjectType() const override;
        virtual const std::string& Name() const override;
        virtual const value_type_t& ValueType() const override;

        virtual bool operator==(const IAttributeDefinition& rhs) const override;
        virtual bool operator!=(const IAttributeDefinition& rhs) const override;

    private:
        std::string _name;
        EObjectType _object_type;
        value_type_t _value_type;
    };
}
