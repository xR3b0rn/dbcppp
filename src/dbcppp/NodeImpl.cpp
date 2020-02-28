
#include "NodeImpl.h"

using namespace dbcppp;

NodeImpl::NodeImpl()
{
}
NodeImpl::~NodeImpl()
{
}
const std::string& NodeImpl::getName() const
{
	return _name;
}
const std::string& NodeImpl::getComment() const
{
	return _comment;
}
const Attribute* NodeImpl::getAttributeValueByName(const std::string& name) const
{
	const Attribute* result = nullptr;
	auto iter = _attribute_values.find(name);
	if (iter != _attribute_values.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const Attribute*>> NodeImpl::getAttributeValues() const
{
	std::vector<std::pair<std::string, const Attribute*>> result;
	for (auto& av : _attribute_values)
	{
		result.emplace_back(av.first, &av.second);
	}
	return result;
}
