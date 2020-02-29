
#include <ostream>
#include "BitTimingImpl.h"

using namespace dbcppp;

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
void BitTiming::serializeToStream(std::ostream& os) const
{
	os << "BS_:";
	if (getBaudrate() != 0 && getBTR1() != 0 && getBTR2() != 0)
	{
		os << " " << getBaudrate() << " : " << getBTR1() << ", " << getBTR2();
	}
}
