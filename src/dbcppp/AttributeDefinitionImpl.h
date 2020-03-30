
#pragma once

#include "../../include/dbcppp/AttributeDefinition.h"

namespace dbcppp
{
    class AttributeDefinitionImpl final
        : public AttributeDefinition
    {
    public:
        AttributeDefinitionImpl(std::string&& name, ObjectType object_type, value_type_t value_type);
        
        virtual std::unique_ptr<AttributeDefinition> clone() const override;

        virtual ObjectType getObjectType() const override;
        virtual const std::string& getName() const override;
        virtual const value_type_t& getValueType() const override;
        virtual void forEachValueTypeEnum(std::function<void(const std::string&)>&& cb) const override;
        virtual const std::string* findValueTypeEnum(std::function<bool(const std::string&)>&& pred) const override;

    private:
        std::string _name;
        ObjectType _object_type;
        value_type_t _value_type;
    };
}
