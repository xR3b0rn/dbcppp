
#include "dbcppp/Network.h"
#include "ValueTableImpl.h"

using namespace dbcppp;

std::unique_ptr<IValueTable> IValueTable::Create(
      std::string&& name
    , std::optional<std::unique_ptr<ISignalType>>&& signal_type
    , std::vector<std::unique_ptr<IValueEncodingDescription>>&& value_encoding_descriptions)
{
    std::optional<SignalTypeImpl> st;
    std::vector<ValueEncodingDescriptionImpl> veds;
    veds.reserve(value_encoding_descriptions.size());
    if (signal_type)
    {
        st = std::move(static_cast<SignalTypeImpl&>(**signal_type));
        (*signal_type).reset(nullptr);
    }
    for (auto& ved : value_encoding_descriptions)
    {
        veds.push_back(std::move(static_cast<ValueEncodingDescriptionImpl&>(*ved)));
        ved.reset(nullptr);
    }
    return std::make_unique<ValueTableImpl>(std::move(name), std::move(st), std::move(veds));
}
ValueTableImpl::ValueTableImpl(
      std::string&& name
    , std::optional<SignalTypeImpl>&& signal_type
    , std::vector<ValueEncodingDescriptionImpl>&& value_encoding_descriptions)

    : _name(std::move(name))
    , _signal_type(std::move(signal_type))
    , _value_encoding_descriptions(std::move(value_encoding_descriptions))
{}
std::unique_ptr<IValueTable> ValueTableImpl::Clone() const
{
    return std::make_unique<ValueTableImpl>(*this);
}
const std::string& ValueTableImpl::Name() const
{
    return _name;
}
std::optional<std::reference_wrapper<const ISignalType>> ValueTableImpl::SignalType() const
{
    std::optional<std::reference_wrapper<const ISignalType>> signal_type;
    if (_signal_type)
    {
        signal_type = *_signal_type;
    }
    return signal_type;
}
const IValueEncodingDescription& ValueTableImpl::ValueEncodingDescriptions_Get(std::size_t i) const
{
    return _value_encoding_descriptions[i];
}
uint64_t ValueTableImpl::ValueEncodingDescriptions_Size() const
{
    return _value_encoding_descriptions.size();
}
bool ValueTableImpl::operator==(const IValueTable& rhs) const
{
    bool equal = true;
    equal &= _name == rhs.Name();
    equal &= _signal_type == rhs.SignalType();
    for (const auto& ved : rhs.ValueEncodingDescriptions())
    {
        auto beg = _value_encoding_descriptions.begin();
        auto end = _value_encoding_descriptions.end();
        equal &= std::find(beg, end, ved) != end;
    }
    return equal;
}
bool ValueTableImpl::operator!=(const IValueTable& rhs) const
{
    return !(*this == rhs);
}