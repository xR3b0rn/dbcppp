
#include "Helper.h"
#include "Node.h"

using namespace dbcppp;

bool SharedNodeCmp::operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const
{
	return lhs->name < rhs->name;
}
