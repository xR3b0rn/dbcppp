
#pragma once

#include <set>
#include <map>
#include <memory>
#include <string>
#include <cstddef>

#include "span.h"
#include "Helper.h"
#include "Node.h"
#include "Attribute.h"

namespace dbcppp
{
	struct Message;
	struct Signal
	{
		enum class Multiplexer
		{
			NoMux, MuxSwitch, MuxValue
		};
		enum class ByteOrder
		{
			LittleEndian, BigEndian
		};
		enum class ValueType
		{
			Signed, Unsigned
		};

		std::shared_ptr<Message> parent_message;
		std::string name;
		Multiplexer multiplexer_indicator;
		uint64_t multiplexer_switch_value;
		uint64_t start_bit;
		uint64_t bit_size;
		ByteOrder byte_order;
		ValueType value_type;
		double factor;
		double offset;
		double minimum;
		double maximum;
		std::string unit;
		std::set<std::shared_ptr<Node>, SharedNodeCmp> receivers;
		std::map<uint64_t, std::string> value_descriptions;
		std::map<std::string, Attribute> attribute_values;
		std::string comment;

		int64_t decode(uint64_t) const;
		void encode(uint64_t* data, int64_t raw) const;
		double raw_to_phys(int64_t raw) const;
		int64_t phys_to_raw(double phys) const;
	};
}
