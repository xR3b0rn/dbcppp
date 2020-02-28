
#pragma once

#include "Attribute.h"

namespace dbcppp
{
	class AttributeImpl
		: public Attribute
	{
	public:
		virtual const std::string& getName() const override;
		virtual AttributeDefinition::ObjectType getObjectType() const override;
		virtual const value_t& getValue() const override;

		std::string _name;
		AttributeDefinition::ObjectType _object_type;
		Attribute::value_t _value;
	};
}
