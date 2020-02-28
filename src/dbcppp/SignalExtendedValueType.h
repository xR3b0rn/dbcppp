
#pragma once

#include <cstddef>
#include <string>

#include "Export.h"

namespace dbcppp
{
    class DBCPPP_API SignalExtendedValueType
    {
    public:
        virtual ~SignalExtendedValueType() = default;
        virtual uint64_t getMessageId() const = 0;
        virtual const std::string& getSignalName() const = 0;
        virtual uint64_t getValue() const = 0;
    };
}