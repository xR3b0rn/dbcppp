
#include "AttributeDefinitionImpl.h"

using namespace dbcppp;

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
