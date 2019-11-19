
#pragma once

#include <map>
#include <string>

#include "Export.h"

namespace dbcppp
{
	struct DBCPPP_EXPORT ValueTable
	{
		std::string name;
		std::map<uint64_t, std::string> value_descriptions;
	};
}
