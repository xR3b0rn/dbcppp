
#pragma once

#include <cstddef>

namespace dbcppp
{
	struct BitTiming
	{
		uint64_t baudrate;
		uint64_t BTR1;
		uint64_t BTR2;
	};
}

