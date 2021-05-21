#pragma once

#include <memory>
#include <cstdint>

#include "Export.h"

namespace dbcppp
{
    class DBCPPP_API IBitTiming
    {
    public:
        static std::unique_ptr<IBitTiming> Create(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2);
        
        virtual std::unique_ptr<IBitTiming> Clone() const = 0;

        virtual ~IBitTiming() = default;
        virtual uint64_t Baudrate() const = 0;
        virtual uint64_t BTR1() const = 0;
        virtual uint64_t BTR2() const = 0;
        
        virtual bool operator==(const IBitTiming& rhs) const = 0;
        virtual bool operator!=(const IBitTiming& rhs) const = 0;
    };
}