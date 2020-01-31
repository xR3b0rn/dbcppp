
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
	// The 8 functions below are used to make the compiler generate 64 * 64 * 2 * 2 = 16.384 decoding functions for any possible combination
	// of aStartBit, aBitSize, aByteOrder and aValueType. The decoding is then as fast as code generated.
	// To reduce the memory usage per compilation unit and so improve the overall perfomance of compiling, the signal generation functions
	// are splitted into more compilation units. Since a if is needed to invoke the correct make_signal_x function, doing this will impact
	// the performance of DBC parsing just a bit.
	//std::shared_ptr<dbcppp::Signal> make_signal_8(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	//std::shared_ptr<dbcppp::Signal> make_signal_16(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	//std::shared_ptr<dbcppp::Signal> make_signal_24(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	//std::shared_ptr<dbcppp::Signal> make_signal_32(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	//std::shared_ptr<dbcppp::Signal> make_signal_40(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	//std::shared_ptr<dbcppp::Signal> make_signal_48(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	//std::shared_ptr<dbcppp::Signal> make_signal_56(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
	//std::shared_ptr<dbcppp::Signal> make_signal_64(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type);
}
