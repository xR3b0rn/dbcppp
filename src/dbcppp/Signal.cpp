
#include "Signal.h"

using namespace dbcppp;

static uint64_t reverse_byte_order(uint64_t x)
{
    x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
    x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
    x = (x & 0x00FF00FF00FF00FF) << 8  | (x & 0xFF00FF00FF00FF00) >> 8;
    return x;
}

uint64_t Signal::decode(uint64_t data) const
{
	uint64_t result = 0;
	uint64_t mask = ~((uint64_t)-1 << bit_size);
	if (byte_order == ByteOrder::BigEndian)
	{
		data = reverse_byte_order(data);
		uint64_t sb = (8 * (7 - (start_bit / 8))) + (start_bit % 8) - (bit_size - 1);
		result = (data >> sb) & mask;
	}
	else
	{
		result = (data >> start_bit) & mask;
	}
	return result;
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
		*data |= raw;
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
