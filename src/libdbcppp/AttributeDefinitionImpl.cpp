
#include <iostream>
#include "AttributeDefinitionImpl.h"

using namespace dbcppp;

std::unique_ptr<AttributeDefinition> AttributeDefinition::create(
      std::string&& name
    , ObjectType object_type
    , value_type_t&& value_type)
{
    return std::make_unique<AttributeDefinitionImpl>(
          std::move(name)
        , object_type
        , std::move(value_type));
}

AttributeDefinitionImpl::AttributeDefinitionImpl(std::string&& name, ObjectType object_type, value_type_t value_type)
    : _name(std::move(name))
    , _object_type(std::move(object_type))
    , _value_type(std::move(value_type))
{}
std::unique_ptr<AttributeDefinition> AttributeDefinitionImpl::clone() const
{
    return std::make_unique<AttributeDefinitionImpl>(*this);
}
AttributeDefinition::ObjectType AttributeDefinitionImpl::getObjectType() const
{
    return _object_type;
}
const std::string& AttributeDefinitionImpl::getName() const
{
    return _name;
}
const AttributeDefinition::value_type_t& AttributeDefinitionImpl::getValueType() const
{
    return _value_type;
}
void AttributeDefinitionImpl::forEachValueTypeEnum(std::function<void(const std::string&)>&& cb) const
{
    for (const auto& v : std::get<ValueTypeEnum>(getValueType()).values)
    {
        cb(v);
    }
}
const std::string* AttributeDefinitionImpl::findValueTypeEnum(std::function<bool(const std::string&)>&& pred) const
{
    const std::string* result = nullptr;
    auto begin = std::get<ValueTypeEnum>(getValueType()).values.cbegin();
    auto end = std::get<ValueTypeEnum>(getValueType()).values.cend();
    auto iter = std::find_if(begin, end, pred);
    if (iter != end)
    {
        result = &*iter;
    }
    return result;
}
