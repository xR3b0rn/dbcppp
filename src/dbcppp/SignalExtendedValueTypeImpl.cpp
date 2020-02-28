#include "SignalExtendedValueTypeImpl.h"

using namespace dbcppp;

const std::string& SignalExtendedValueTypeImpl::getSignalName() const
{
    return _signal_name;
}

uint64_t SignalExtendedValueTypeImpl::getMessageId() const
{
    return _message_id;
}

uint64_t SignalExtendedValueTypeImpl::getValue() const
{
    return _value;
}