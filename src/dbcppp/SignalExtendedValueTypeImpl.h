#pragma once

#include "SignalExtendedValueType.h"

namespace dbcppp
{
    class SignalExtendedValueTypeImpl
            : public SignalExtendedValueType
    {
    public:
        virtual uint64_t getMessageId() const override;
        virtual const std::string& getSignalName() const override;
        virtual uint64_t getValue() const override;

        uint64_t _message_id;
        std::string _signal_name;
        uint64_t _value;
    };
}