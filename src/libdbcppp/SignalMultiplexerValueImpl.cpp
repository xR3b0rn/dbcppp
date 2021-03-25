
#include "SignalMultiplexerValueImpl.h"

using namespace dbcppp;

std::unique_ptr<SignalMultiplexerValue> SignalMultiplexerValue::create(
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
            
std::unique_ptr<SignalMultiplexerValue> SignalMultiplexerValueImpl::clone() const
{
    return nullptr;
}

std::string SignalMultiplexerValueImpl::getSwitchName() const
{
    return _switch_name;
}
void SignalMultiplexerValueImpl::forEachValueRange(std::function<void(const Range&)> cb) const
{
    for (const auto& value_range : _value_ranges)
    {
        cb(value_range);
    }
}

bool SignalMultiplexerValueImpl::operator==(const SignalMultiplexerValue& rhs) const
{
    bool result = true;
    result &= _switch_name == rhs.getSwitchName();
    return result;
}
bool SignalMultiplexerValueImpl::operator!=(const SignalMultiplexerValue& rhs) const
{
    return !(*this == rhs);
}
