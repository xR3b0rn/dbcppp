
#include "../../include/dbcppp/Network.h"
#include "ValueTableImpl.h"

using namespace dbcppp;

std::unique_ptr<ValueTable> ValueTable::create(
      std::string&& name
    , std::optional<std::unique_ptr<SignalType>>&& signal_type
    , std::vector<std::tuple<int64_t, std::string>>&& value_encoding_descriptions)
{
    std::optional<SignalTypeImpl> st;
    if (signal_type)
    {
        st = std::move(static_cast<SignalTypeImpl&>(**signal_type));
        (*signal_type).reset(nullptr);
    }
    return std::make_unique<ValueTableImpl>(std::move(name), std::move(st), std::move(value_encoding_descriptions));
}
ValueTableImpl::ValueTableImpl(
      std::string&& name
    , std::optional<SignalTypeImpl>&& signal_type
    , std::vector<std::tuple<int64_t, std::string>>&& value_encoding_descriptions)

    : _name(std::move(name))
    , _signal_type(std::move(signal_type))
    , _value_encoding_descriptions(std::move(value_encoding_descriptions))
{}
std::unique_ptr<ValueTable> ValueTableImpl::clone() const
{
    return std::make_unique<ValueTableImpl>(*this);
}
const std::string& ValueTableImpl::getName() const
{
    return _name;
}
std::optional<std::reference_wrapper<const SignalType>> ValueTableImpl::getSignalType() const
{
    std::optional<std::reference_wrapper<const SignalType>> result;
    if (_signal_type)
    {
        result = *_signal_type;
    }
    return result;
}
const std::string* ValueTableImpl::getvalueEncodingDescriptionByValue(int64_t value) const
{
    const std::string* result = nullptr;
    auto iter = std::find_if(_value_encoding_descriptions.begin(), _value_encoding_descriptions.end(),
        [&](const auto& ved) { return std::get<0>(ved) == value; });
    if (iter != _value_encoding_descriptions.end())
    {
        result = &std::get<1>(*iter);
    }
    return result;
}
void ValueTableImpl::forEachValueEncodingDescription(std::function<void(int64_t, const std::string&)>&& cb) const
{
    for (const auto& ved : _value_encoding_descriptions)
    {
        cb(std::get<0>(ved), std::get<1>(ved));
    }
}

bool ValueTableImpl::operator==(const ValueTable& rhs) const
{
    bool result = true;
    result &= _name == rhs.getName();
    result &= _signal_type == rhs.getSignalType();
    rhs.forEachValueEncodingDescription(
        [&](int64_t value, const std::string& desc)
        {
            auto beg = _value_encoding_descriptions.begin();
            auto end = _value_encoding_descriptions.end();
            result &= std::find(beg, end, std::make_tuple(value, desc)) != end;
        });
    return result;
}
bool ValueTableImpl::operator!=(const ValueTable& rhs) const
{
    return !(*this == rhs);
}
