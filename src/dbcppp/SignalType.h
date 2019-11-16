
#pragma once

#include <cstddef>
#include <string>

#include "Signal.h"

namespace dbcppp
{
	struct SignalType
	{
		std::string name;
		uint64_t signal_size;
		Signal::ByteOrder byte_order;
		Signal::ValueType value_type;
		double factor;
		double offset;
		double minimum;
		double maximum;
		std::string unit;
		double default_value;
		std::string value_table; // maybe std::shared_ptr to ValueTable
	};
}
