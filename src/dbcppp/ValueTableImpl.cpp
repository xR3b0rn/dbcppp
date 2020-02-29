
#include "Network.h"
#include "ValueTableImpl.h"

using namespace dbcppp;

const std::string& ValueTableImpl::getName() const
{
	return _name;
}
boost::optional<const SignalType&> ValueTableImpl::getSignalType() const
{
	if (_signal_type)
	{
		return *_signal_type;
	}
	return boost::none;
}
std::vector<std::pair<double, const std::string*>> ValueTableImpl::getValueDescriptions() const
{
	std::vector<std::pair<double, const std::string*>> result;
	for (const auto& vd : _value_descriptions)
	{
		result.emplace_back(vd.first, &vd.second);
	}
	return result;
}
const std::string* ValueTableImpl::getValueDescriptionById(double id) const
{
	const std::string* result = nullptr;
	auto iter = _value_descriptions.find(id);
	if (iter != _value_descriptions.end())
	{
		result = &iter->second;
	}
	return result;
}

void ValueTable::serializeToStream(std::ostream& os) const
{
	auto value_descriptions = getValueDescriptions();
	if (value_descriptions.size())
	{
		os << "VAL_TABLE_ " << getName();
		for (const auto& ved : getValueDescriptions())
		{
			os << " " << ved.first << " \"" << *ved.second << "\"";
		}
		os << ";";
	}
}
