
#pragma once

#include <string>
#include <memory>

namespace dbcppp
{
	struct Node;
	struct SharedNodeCmp
	{
		bool operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const;
	};
}
bool operator==(const dbcppp::Node& lhs, std::string rhs);
