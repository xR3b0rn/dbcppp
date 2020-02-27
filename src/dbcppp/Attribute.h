
#pragma once

#include <string>
#include <cstddef>
#include <boost/variant.hpp>

#include "Export.h"
#include "AttributeDefinition.h"

namespace dbcppp
{
	class DBCPPP_API Attribute
	{
	public:
		struct IntegerValue { int64_t value; };
		struct HexValue { int64_t value; };
		struct FloatValue { double value; };
		struct EnumValue { int64_t value; };
		struct StringValue { std::string value; };

		using hex_value_t = int64_t;
		using value_t = boost::variant<IntegerValue, HexValue, FloatValue, EnumValue, StringValue>;

		virtual ~Attribute() = default;
		virtual const std::string& getName() const = 0;
		virtual AttributeDefinition::ObjectType getObjectType() const = 0;
		virtual const value_t& getValue() const = 0;
	};
}
