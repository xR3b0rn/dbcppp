
#include "MakeSignal.h"
#include "../dbcppp/Network.h"

using namespace dbcppp;

template <uint64_t aStartBit, uint64_t aBitSize, dbcppp::Signal::ByteOrder aByteOrder, dbcppp::Signal::ValueType aValueType>
struct MakeSignal24
{
	static std::shared_ptr<dbcppp::Signal> signal(uint64_t start_bit, uint64_t bit_size, dbcppp::Signal::ByteOrder byte_order, dbcppp::Signal::ValueType value_type)
	{
		if (aStartBit == start_bit && aBitSize == bit_size && aByteOrder == byte_order && aValueType == value_type)
		{
			return std::make_shared<TemplateSignal<aStartBit, aBitSize, aByteOrder, aValueType>>();
		}
		if constexpr (aValueType == dbcppp::Signal::ValueType::Signed)
		{
			return MakeSignal24<aStartBit, aBitSize, aByteOrder, dbcppp::Signal::ValueType::Unsigned>::signal(start_bit, bit_size, byte_order, value_type);
		}
		if constexpr (aByteOrder == dbcppp::Signal::ByteOrder::BigEndian)
		{
			return MakeSignal24<aStartBit, aBitSize, dbcppp::Signal::ByteOrder::LittleEndian, dbcppp::Signal::ValueType::Signed>::signal(start_bit, bit_size, byte_order, value_type);
		}
		if constexpr (aBitSize > 1)
		{
			return MakeSignal24<aStartBit, aBitSize - 1, dbcppp::Signal::ByteOrder::BigEndian, dbcppp::Signal::ValueType::Signed>::signal(start_bit, bit_size, byte_order, value_type);
		}
		if constexpr (aStartBit > 17)
		{
			return MakeSignal24<aStartBit - 1, 63, dbcppp::Signal::ByteOrder::BigEndian, dbcppp::Signal::ValueType::Signed>::signal(start_bit, bit_size, byte_order, value_type);
		}
		return std::make_shared<dbcppp::Signal>();
	}
};

std::shared_ptr<dbcppp::Signal> dbcppp::make_signal_24(uint64_t start_bit, uint64_t bit_size, Signal::ByteOrder byte_order, Signal::ValueType value_type)
{
	return MakeSignal24<24, 63, dbcppp::Signal::ByteOrder::BigEndian, dbcppp::Signal::ValueType::Signed>::signal(start_bit, bit_size, byte_order, value_type);
}
