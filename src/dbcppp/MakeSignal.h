
#pragma once

#include <boost/endian/conversion.hpp>

#include "Export.h"
#include "Network.h"

namespace dbcppp
{
	DBCPPP_EXPORT std::shared_ptr<dbcppp::Signal> make_signal(Signal::ByteOrder byte_order, Signal::ValueType value_type);
}
