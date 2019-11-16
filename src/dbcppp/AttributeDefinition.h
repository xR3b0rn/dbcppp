
#pragma once

#include <cstddef>
#include <string>
#include <Vector>
#include <boost/variant.hpp>

namespace dbcppp
{
	struct AttributeDefinition
	{
		enum class ObjectType
		{
			Network,
			Node,
			Message,
			Signal,
			EnvironmentVariable,
		};
		struct ValueTypeInt
		{
			int64_t minimum;
			int64_t maximum;
		};
		struct ValueTypeHex
		{
			int64_t minimum;
			int64_t maximum;
		};
		struct ValueTypeFloat
		{
			double minimum;
			double maximum;
		};
		struct ValueTypeString
		{
		};
		struct ValueTypeEnum
		{
			std::vector<std::string> values;
		};
		using value_type_t = boost::variant<ValueTypeInt, ValueTypeHex, ValueTypeFloat, ValueTypeString, ValueTypeEnum>;
		
		ObjectType object_type;
		std::string name;
		value_type_t value_type;
	};
}
