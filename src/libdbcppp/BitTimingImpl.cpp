#include <ostream>
#include "BitTimingImpl.h"

using namespace dbcppp;

std::unique_ptr<BitTiming> BitTiming::create(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2)
{
    return std::make_unique<BitTimingImpl>(baudrate, BTR1, BTR2);
}
BitTimingImpl::BitTimingImpl()
    : _baudrate(0)
    , _BTR1(0)
    , _BTR2(0)
{}
BitTimingImpl::BitTimingImpl(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2)
    : _baudrate(std::move(baudrate))
    , _BTR1(std::move(BTR1))
    , _BTR2(std::move(BTR2))
{}
std::unique_ptr<BitTiming> BitTimingImpl::clone() const
{
    return std::make_unique<BitTimingImpl>(*this);
}
uint64_t BitTimingImpl::getBaudrate() const
{
    return _baudrate;
}
uint64_t BitTimingImpl::getBTR1() const
{
    return _BTR1;
}
uint64_t BitTimingImpl::getBTR2() const
{
    return _BTR2;
}
bool BitTimingImpl::operator==(const BitTiming& rhs) const
{
    bool result = true;
    result &= _baudrate == rhs.getBaudrate();
    result &= _BTR1 == rhs.getBTR1();
    result &= _BTR2 == rhs.getBTR2();
    return result;
}
bool BitTimingImpl::operator!=(const BitTiming& rhs) const
{
    return !(*this == rhs);
}