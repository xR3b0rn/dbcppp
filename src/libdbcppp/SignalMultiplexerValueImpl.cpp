
#include "SignalMultiplexerValueImpl.h"

using namespace dbcppp;

std::unique_ptr<ISignalMultiplexerValue> ISignalMultiplexerValue::Create(
      std::string&& switch_name
    , std::vector<Range>&& value_ranges)
{
    return std::make_unique<SignalMultiplexerValueImpl>(
          std::move(switch_name)
        , std::move(value_ranges));
}

SignalMultiplexerValueImpl::SignalMultiplexerValueImpl(
      std::string&& switch_name
    , std::vector<Range>&& value_ranges)

    : _switch_name(std::move(switch_name))
    , _value_ranges(std::move(value_ranges))
{}
            
std::unique_ptr<ISignalMultiplexerValue> SignalMultiplexerValueImpl::Clone() const
{
    return std::make_unique<SignalMultiplexerValueImpl>(*this);
}

const std::string& SignalMultiplexerValueImpl::SwitchName() const
{
    return _switch_name;
}
const ISignalMultiplexerValue::Range& SignalMultiplexerValueImpl::ValueRanges_Get(std::size_t i) const
{
    return _value_ranges[i];
}
uint64_t SignalMultiplexerValueImpl::ValueRanges_Size() const
{
    return _value_ranges.size();
}

bool SignalMultiplexerValueImpl::operator==(const ISignalMultiplexerValue& rhs) const
{
    bool equal = true;
    equal &= _switch_name == rhs.SwitchName();
    for (const auto& range : rhs.ValueRanges())
    {
        auto beg = _value_ranges.begin();
        auto end = _value_ranges.end();
        equal &= std::find_if(beg, end,
            [&](const auto& other) { return range.from == other.from && range.to == other.to; }) != end;
    }
    return equal;
}
bool SignalMultiplexerValueImpl::operator!=(const ISignalMultiplexerValue& rhs) const
{
    return !(*this == rhs);
}
