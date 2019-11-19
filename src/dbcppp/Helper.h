
#pragma once

#include <string>
#include <memory>

#include "Export.h"

namespace dbcppp
{
	struct Node;
	struct DBCPPP_EXPORT SharedNodeCmp
	{
		bool operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const;
	};
}
bool operator==(const dbcppp::Node& lhs, std::string rhs);
