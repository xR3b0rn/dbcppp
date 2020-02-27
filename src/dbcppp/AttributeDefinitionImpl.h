
#pragma once

#include "AttributeDefinition.h"

namespace dbcppp
{
	class AttributeDefinitionImpl
		: public AttributeDefinition
	{
	public:
		virtual ObjectType getObjectType() const override;
		virtual const std::string& getName() const override;
		virtual const value_type_t& getValueType() const override;

		std::string _name;
		AttributeDefinition::ObjectType _object_type;
		AttributeDefinition::value_type_t _value_type;
	};
}
