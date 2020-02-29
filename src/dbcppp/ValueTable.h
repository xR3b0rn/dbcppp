
#pragma once

#include <vector>
#include <string>

#include "Export.h"

namespace dbcppp
{
	class Network;
	class DBCPPP_API ValueTable
	{
	public:
		virtual ~ValueTable() = default;
		virtual const std::string& getName() const = 0;
		virtual std::vector<std::pair<uint64_t, const std::string*>> getValueDescriptions() const = 0;
		virtual const std::string* getValueDescriptionById(uint64_t id) const = 0;
		
		void serializeToStream(std::ostream& os) const;
	};
}
