
#include "Helper.h"
#include "NodeImpl.h"

using namespace dbcppp;

bool SharedNodeCmp::operator()(const NodeImpl& lhs, const NodeImpl& rhs) const
{
	return lhs._name < rhs._name;
}
