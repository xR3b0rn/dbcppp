
#pragma once

#include "BitTiming.h"

namespace dbcppp
{
	class BitTimingImpl
		: public BitTiming
	{
	public:
		virtual uint64_t getBaudrate() const override;
		virtual uint64_t getBTR1() const override;
		virtual uint64_t getBTR2() const override;

		uint64_t _baudrate;
		uint64_t _BTR1;
		uint64_t _BTR2;
	};
}
