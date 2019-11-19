
#include "Signal.h"
#include <boost/endian/conversion.hpp>

using namespace dbcppp;

uint64_t Signal::decode(uint64_t data) const
{
	uint64_t result = 0;
	uint64_t mask = ~((uint64_t)-1 << bit_size);
	if (byte_order == ByteOrder::BigEndian)
	{
		data = boost::endian::endian_reverse(data);
		uint64_t sb = (8 * (7 - (start_bit / 8))) + (start_bit % 8) - (bit_size - 1);
		data >>= sb;
		if (value_type == ValueType::Signed && (data & (1ull << (bit_size - 1))))
		{
			data |= ~mask;
		}
		else
		{
			data &= mask;
		}
	}
	else
	{
		data >>= start_bit;
		if (value_type == ValueType::Signed && (data & (1ull << (bit_size - 1))))
		{
			data |= ~mask;
		}
		else
		{
			data &= mask;
		}
	}
	return data;
}
void Signal::encode(uint64_t* data, uint64_t raw) const
{
	uint64_t mask = ~((uint64_t)-1 << bit_size);
	raw &= mask;
	if (byte_order == ByteOrder::BigEndian)
	{
		uint64_t sb = (8 * (7 - (start_bit / 8))) + (start_bit % 8) - (bit_size - 1);
		raw <<= sb;
		*data &= ~(mask << sb);
		*data |= boost::endian::endian_reverse(raw);
	}
	else
	{
		*data &= ~(mask << start_bit);
		*data |= raw << start_bit;
	}
}
double Signal::raw_to_phys(uint64_t raw) const
{
	return raw * factor + offset;
}
uint64_t Signal::phys_to_raw(double phys) const
{
	return (phys - offset) / factor;
}
