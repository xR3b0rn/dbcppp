
#include "AttributeImpl.h"

using namespace dbcppp;

const std::string& AttributeImpl::getName() const
{
	return _name;
}
AttributeDefinition::ObjectType AttributeImpl::getObjectType() const
{
	return _object_type;
}
const Attribute::value_t& AttributeImpl::getValue() const
{
	return _value;
}
