
#include "ValueTableImpl.h"

using namespace dbcppp;

const std::string& ValueTableImpl::getName() const
{
	return _name;
}
std::vector<std::pair<uint64_t, const std::string*>> ValueTableImpl::getValueDescriptions() const
{
	std::vector<std::pair<uint64_t, const std::string*>> result;
	for (const auto& vd : _value_descriptions)
	{
		result.emplace_back(vd.first, &vd.second);
	}
	return result;
}
const std::string* ValueTableImpl::getValueDescriptionById(uint64_t id) const
{
	const std::string* result = nullptr;
	auto iter = _value_descriptions.find(id);
	if (iter != _value_descriptions.end())
	{
		result = &iter->second;
	}
	return result;
}
