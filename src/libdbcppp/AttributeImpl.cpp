#include "AttributeImpl.h"
#include "../../include/dbcppp/Network.h"

using namespace dbcppp;

std::unique_ptr<Attribute> Attribute::create(
      std::string&& name
    , AttributeDefinition::ObjectType object_type
    , value_t value)
{
    return std::make_unique<AttributeImpl>(
          std::move(name)
        , object_type
        , std::move(value));
}

AttributeImpl::AttributeImpl(std::string&& name, AttributeDefinition::ObjectType object_type, Attribute::value_t value)
    : _name(std::move(name))
    , _object_type(std::move(object_type))
    , _value(std::move(value))
{}
std::unique_ptr<Attribute> AttributeImpl::clone() const
{
    return std::make_unique<AttributeImpl>(*this);
}
const std::string& AttributeImpl::getName() const
{
    return _name;
}
AttributeDefinition::ObjectType AttributeImpl::getObjectType() const
{
    return _object_type;
};
const Attribute::value_t& AttributeImpl::getValue() const
{
    return _value;
}
bool AttributeImpl::operator==(const Attribute& rhs) const
{
    bool result = true;
    result &= _name == rhs.getName();
    result &= _object_type == rhs.getObjectType();
    result &= _value == rhs.getValue();
    return result;
}
bool AttributeImpl::operator!=(const Attribute& rhs) const
{
    return !(*this == rhs);
}