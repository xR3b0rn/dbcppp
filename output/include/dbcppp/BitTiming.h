#pragma once

#include <memory>
#include <cstdint>

#include "Export.h"

namespace dbcppp
{
    class Network;
    class DBCPPP_API BitTiming
    {
    public:
        static std::unique_ptr<BitTiming> create(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2);
        
        virtual std::unique_ptr<BitTiming> clone() const = 0;

        virtual ~BitTiming() = default;
        virtual uint64_t getBaudrate() const = 0;
        virtual uint64_t getBTR1() const = 0;
        virtual uint64_t getBTR2() const = 0;
    };
}

