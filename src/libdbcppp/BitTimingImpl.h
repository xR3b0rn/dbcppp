
#pragma once

#include "../../include/dbcppp/BitTiming.h"

namespace dbcppp
{
    class BitTimingImpl final
        : public BitTiming
    {
    public:
        BitTimingImpl();
        BitTimingImpl(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2);
        
        virtual std::unique_ptr<BitTiming> clone() const override;

        virtual uint64_t getBaudrate() const override;
        virtual uint64_t getBTR1() const override;
        virtual uint64_t getBTR2() const override;

    private:
        uint64_t _baudrate;
        uint64_t _BTR1;
        uint64_t _BTR2;
    };
}
