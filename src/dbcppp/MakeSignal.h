
#pragma once

#include <boost/endian/conversion.hpp>

#include "Network.h"

namespace dbcppp
{
	template <uint64_t aStartBit, uint64_t aBitSize, dbcppp::Signal::ByteOrder aByteOrder, dbcppp::Signal::ValueType aValueType>
	struct TemplateSignal
		: dbcppp::Signal
	{
		virtual uint64_t decode(uint64_t data) const override
		{
			if constexpr (aByteOrder == ByteOrder::BigEndian)
			{
				data = boost::endian::endian_reverse(data);
				constexpr uint64_t fixed_start_bit = (8 * (7 - (aStartBit / 8))) + (aStartBit % 8) - (aBitSize - 1);
				constexpr uint64_t mask = ~((uint64_t)-1 << fixed_start_bit);
				data >>= fixed_start_bit;
				if constexpr (aValueType == ValueType::Signed)
				{
					constexpr uint64_t control_bit = 1ull << (aBitSize - 1);
					if (data & control_bit) data |= ~mask;
					else data &= mask;
				}
				else
				{
					data &= mask;
				}
			}
			else
			{
				constexpr uint64_t mask = ~((uint64_t)-1 << aStartBit);
				data >>= aStartBit;
				if constexpr (aValueType == ValueType::Signed)
				{
					constexpr uint64_t control_bit = 1ull << (aBitSize - 1);
					if (data & control_bit) data |= ~mask;
					else data &= mask;
				}
				else data &= mask;
			}
			return data;
		}
	};
	// The 8 functions below are used to make the compiler generate 64 * 64 * 2 * 2 = 16.384 decoding functions for any possible combination
	// of aStartBit, aBitSize, aByteOrder and aValueType. The decoding is then as fast as code generated.
	// To reduce the memory usage per compilation unit and so improve the overall perfomance of compiling, the signal generation functions are splitted
	// into more compilation units. Doing this only impacts the performance of DBC parsing a bit, since a if is needed to invoke the correct
	// make_signal_x function.
	std::shared_ptr<dbcppp::Signal> make_signal_8(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	std::shared_ptr<dbcppp::Signal> make_signal_16(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	std::shared_ptr<dbcppp::Signal> make_signal_24(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	std::shared_ptr<dbcppp::Signal> make_signal_32(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	std::shared_ptr<dbcppp::Signal> make_signal_40(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	std::shared_ptr<dbcppp::Signal> make_signal_48(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	std::shared_ptr<dbcppp::Signal> make_signal_56(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	std::shared_ptr<dbcppp::Signal> make_signal_64(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
}
