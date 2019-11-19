
#pragma once

#include <map>
#include <string>
#include "Export.h"
#include "Attribute.h"

namespace dbcppp
{
	struct DBCPPP_EXPORT Node
	{
		std::string name;
		std::map<std::string, Attribute> attribute_values;
		std::string comment;
	};
}
