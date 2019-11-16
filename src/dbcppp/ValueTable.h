
#pragma once

#include <map>
#include <string>

namespace dbcppp
{
	struct ValueTable
	{
		std::string name;
		std::map<uint64_t, std::string> value_descriptions;
	};
}
