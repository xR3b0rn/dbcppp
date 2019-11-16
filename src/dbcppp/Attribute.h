
#pragma once

#include <string>
#include <cstddef>
#include <boost/variant.hpp>

#include "AttributeDefinition.h"

namespace dbcppp
{
	struct Attribute
	{
		std::string name;
		AttributeDefinition::ObjectType object_type; // maybe std::shared_ptr to AttributeDefinition
		struct IntegerValue { int64_t value; };
		struct HexValue { int64_t value; };
		struct FloatValue { double value; };
		struct EnumValue { int64_t value; };
		struct StringValue { std::string value; };
		using hex_value_t = int64_t;
		using value_t = boost::variant<IntegerValue, HexValue, FloatValue, EnumValue, StringValue>;
		value_t value;
	};
}
