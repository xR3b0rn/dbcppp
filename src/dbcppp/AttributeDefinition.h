
#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <boost/variant.hpp>

#include "Export.h"

namespace dbcppp
{
	class Network;
	class DBCPPP_API AttributeDefinition
	{
	public:
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
		
		virtual ~AttributeDefinition() = default;
		virtual ObjectType getObjectType() const = 0;
		virtual const std::string& getName() const = 0;
		virtual const value_type_t& getValueType() const = 0;

		void serializeToStream(std::ostream& os, const Network& net) const;
	};
}
