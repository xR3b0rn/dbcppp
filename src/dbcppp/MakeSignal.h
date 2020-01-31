
#pragma once

#include <boost/endian/conversion.hpp>

#include "Network.h"

namespace dbcppp
{
	template <dbcppp::Signal::ByteOrder aByteOrder, dbcppp::Signal::ValueType aValueType>
	struct TemplateSignal
		: dbcppp::Signal
	{
		virtual uint64_t decode(uint64_t data) const override
		{
			if constexpr (aByteOrder == ByteOrder::BigEndian)
			{
				data = boost::endian::endian_reverse(data);
			}
			data >>= fixed_start_bit;
			if constexpr (aValueType == ValueType::Signed)
			{
				// bit extending
				data |= ~(data & mask_signed);
			}
			else
			{
				data &= mask;
			}
			return data;
		}
	};
	std::shared_ptr<dbcppp::Signal> make_signal(Signal::ByteOrder byte_order, Signal::ValueType value_type);
}
