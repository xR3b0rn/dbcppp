
#include "Helper.h"
#include "Node.h"

using namespace dbcppp;

bool SharedNodeCmp::operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const
{
	return lhs->name < rhs->name;
}
bool operator==(const std::shared_ptr<Node>& lhs, const std::string& rhs)
{
	return lhs->name == rhs;
}
bool operator==(const std::string& lhs, const std::shared_ptr<Node>& rhs)
{
	return lhs == rhs->name;
}
