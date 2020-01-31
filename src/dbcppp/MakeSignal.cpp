
#include "MakeSignal.h"
#include "../dbcppp/Network.h"

using namespace dbcppp;

template <dbcppp::Signal::ByteOrder aByteOrder, dbcppp::Signal::ValueType aValueType>
struct MakeSignal
{
	static std::shared_ptr<dbcppp::Signal> signal(dbcppp::Signal::ByteOrder byte_order, dbcppp::Signal::ValueType value_type)
	{
		if (aByteOrder == byte_order && aValueType == value_type)
		{
			return std::make_shared<TemplateSignal<aByteOrder, aValueType>>();
		}
		if constexpr (aValueType == dbcppp::Signal::ValueType::Signed)
		{
			return MakeSignal<aByteOrder, dbcppp::Signal::ValueType::Unsigned>::signal(byte_order, value_type);
		}
		if constexpr (aByteOrder == dbcppp::Signal::ByteOrder::BigEndian)
		{
			return MakeSignal<dbcppp::Signal::ByteOrder::LittleEndian, dbcppp::Signal::ValueType::Signed>::signal(byte_order, value_type);
		}
		return std::make_shared<dbcppp::Signal>();
	}
};

std::shared_ptr<dbcppp::Signal> dbcppp::make_signal(Signal::ByteOrder byte_order, Signal::ValueType value_type)
{
	return MakeSignal<dbcppp::Signal::ByteOrder::BigEndian, dbcppp::Signal::ValueType::Signed>::signal(byte_order, value_type);
}
