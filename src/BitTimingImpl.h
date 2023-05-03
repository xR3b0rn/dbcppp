#pragma once

#include "dbcppp/BitTiming.h"

namespace dbcppp
{
    class BitTimingImpl final
        : public IBitTiming
    {
    public:
        BitTimingImpl();
        BitTimingImpl(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2);
        
        virtual std::unique_ptr<IBitTiming> Clone() const override;

        virtual uint64_t Baudrate() const override;
        virtual uint64_t BTR1() const override;
        virtual uint64_t BTR2() const override;
        
        virtual bool operator==(const IBitTiming& rhs) const override;
        virtual bool operator!=(const IBitTiming& rhs) const override;

    private:
        uint64_t _baudrate;
        uint64_t _BTR1;
        uint64_t _BTR2;
    };
}