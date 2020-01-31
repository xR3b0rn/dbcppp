
#pragma once

#include <boost/endian/conversion.hpp>

#include "Network.h"

namespace dbcppp
{
	std::shared_ptr<dbcppp::Signal> make_signal(Signal::ByteOrder byte_order, Signal::ValueType value_type);
}
