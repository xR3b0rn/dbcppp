
#pragma once

#include "Export.h"
#include "Network.h"

namespace dbcppp
{
	DBCPPP_API std::shared_ptr<dbcppp::Signal> make_signal(Signal::ByteOrder byte_order, Signal::ValueType value_type);
}
