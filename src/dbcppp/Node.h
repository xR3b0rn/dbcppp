
#pragma once

#include <map>
#include <string>
#include "Attribute.h"

namespace dbcppp
{
	struct Node
	{
		std::string name;
		std::map<std::string, Attribute> attribute_values;
		std::string comment;
	};
}
