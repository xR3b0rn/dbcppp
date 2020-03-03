
#include "Network.h"
#include "ValueTableImpl.h"

using namespace dbcppp;

std::unique_ptr<ValueTable> ValueTable::create(
	  std::string&& name
	, boost::optional<std::unique_ptr<SignalType>>&& signal_type
	, std::map<double, std::string>&& value_encoding_descriptions)
{
	boost::optional<SignalTypeImpl> st;
	if (signal_type)
	{
		st = std::move(static_cast<SignalTypeImpl&>(**signal_type));
		(*signal_type).reset(nullptr);
	}
	return std::make_unique<ValueTableImpl>(std::move(name), std::move(st), std::move(value_encoding_descriptions));
}
ValueTableImpl::ValueTableImpl(
	  std::string&& name
	, boost::optional<SignalTypeImpl>&& signal_type
	, std::map<double, std::string>&& value_encoding_descriptions)

	: _name(std::move(name))
	, _signal_type(std::move(signal_type))
	, _value_encoding_descriptions(std::move(value_encoding_descriptions))
{}
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
std::vector<std::pair<double, const std::string*>> ValueTableImpl::getValueEncodingDescriptions() const
{
	std::vector<std::pair<double, const std::string*>> result;
	for (const auto& ved : _value_encoding_descriptions)
	{
		result.emplace_back(std::make_pair(ved.first, &ved.second));
	}
	return result;
}
const std::string* ValueTableImpl::getValueEncodingDescriptions(double value) const
{
	const std::string* result = nullptr;
	auto iter = _value_encoding_descriptions.find(value);
	if (iter != _value_encoding_descriptions.end())
	{
		result = &iter->second;
	}
	return result;
}

void ValueTable::serializeToStream(std::ostream& os) const
{
	auto veds = getValueEncodingDescriptions();
	if (veds.size())
	{
		os << "VAL_TABLE_ " << getName();
		for (const auto& ved : veds)
		{
			os << " " << ved.first << " \"" << *ved.second << "\"";
		}
		os << ";";
	}
}
