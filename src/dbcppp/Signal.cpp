
#include "Signal.h"
#include <boost/endian/conversion.hpp>

using namespace dbcppp;


Signal::raw_t Signal::decode8(const void* _8byte) const noexcept
{
	return _decode8(*this, _8byte);
}
Signal::raw_t Signal::decode64(const void* _64byte) const noexcept
{
	return _decode64(*this, _64byte);
}
void Signal::encode(uint64_t* data, int64_t raw) const
{
	raw &= mask;
	if (byte_order == ByteOrder::BigEndian)
	{
		raw <<= fixed_start_bit;
		*data &= ~(mask << fixed_start_bit);
		*data |= boost::endian::endian_reverse(raw);
	}
	else
	{
		*data &= ~(mask << fixed_start_bit);
		*data |= raw << fixed_start_bit;
	}
}
double Signal::raw_to_phys(int64_t raw) const
{
	return double(raw) * factor + offset;
}
int64_t Signal::phys_to_raw(double phys) const
{
	return (phys - offset) / factor;
}
void Signal::fix_performance_attributes()
{
	mask = (1ull << bit_size) - 1;
	mask_signed = (1ull << (bit_size - 1));
	fixed_start_bit =
		  byte_order == dbcppp::Signal::ByteOrder::BigEndian
		? (8 * (7 - (start_bit / 8))) + (start_bit % 8) - (bit_size - 1)
		: start_bit;
	fixed_start_bit_fd =
		  (byte_order == dbcppp::Signal::ByteOrder::BigEndian
		? (8 * (63 - (start_bit / 8))) + (start_bit % 8) - (bit_size - 1)
		: start_bit);
	byte_pos_fd = fixed_start_bit_fd / 8;
	fixed_start_bit_fd -= byte_pos_fd * 8;
}
