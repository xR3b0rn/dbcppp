
#pragma once

#include <cstddef>

#include "Export.h"

namespace dbcppp
{
	struct DBCPPP_EXPORT BitTiming
	{
		uint64_t baudrate;
		uint64_t BTR1;
		uint64_t BTR2;
	};
}

