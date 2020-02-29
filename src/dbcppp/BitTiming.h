
#pragma once

#include <memory>
#include <cstdint>

#include "Export.h"

namespace dbcppp
{
	class Network;
	class DBCPPP_API BitTiming
	{
	public:
		virtual ~BitTiming() = default;
		virtual uint64_t getBaudrate() const = 0;
		virtual uint64_t getBTR1() const = 0;
		virtual uint64_t getBTR2() const = 0;
		
		void serializeToStream(std::ostream& os, const Network& net) const;
	};
}
