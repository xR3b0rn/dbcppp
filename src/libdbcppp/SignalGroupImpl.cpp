
#include "SignalGroupImpl.h"

using namespace dbcppp;

std::unique_ptr<SignalGroup> SignalGroup::create(
      uint64_t message_id
    , std::string&& name
    , uint64_t repetitions
    , std::vector<std::string>&& signal_names)
{
    return std::make_unique<SignalGroupImpl>(
          message_id
        , std::move(name)
        , repetitions
        , std::move(signal_names));
}

std::unique_ptr<SignalGroup> SignalGroupImpl::clone() const
{
    return std::make_unique<SignalGroupImpl>(*this);
}

SignalGroupImpl::SignalGroupImpl(
      uint64_t message_id
    , std::string&& name
    , uint64_t repetitions
    , std::vector<std::string>&& signal_names)

    : _message_id(message_id)
    , _name(std::move(name))
    , _repetitions(repetitions)
    , _signal_names(std::move(signal_names))
{}
uint64_t SignalGroupImpl::getMessageId() const
{
    return _message_id;
}
const std::string& SignalGroupImpl::getName() const
{
    return _name;
}
uint64_t SignalGroupImpl::getRepetitions() const
{
    return _repetitions;
}
void SignalGroupImpl::forEachSignalName(std::function<void(const std::string&)> cb) const
{
    for (const auto& signal_name : _signal_names)
    {
        cb(signal_name);
    }
}
        
bool SignalGroupImpl::operator==(const SignalGroup& rhs) const
{
    bool result = true;
    result &= _message_id == rhs.getMessageId();
    result &= _name == rhs.getName();
    result &= _repetitions== rhs.getRepetitions();
    rhs.forEachSignalName(
        [&](const std::string& signal_name)
        {
            auto beg = _signal_names.begin();
            auto end = _signal_names.end();
            result &= std::find(beg, end, signal_name) != end;
        });
    return result;
}
bool SignalGroupImpl::operator!=(const SignalGroup& rhs) const
{
    return !(*this == rhs);
}
