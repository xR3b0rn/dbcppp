
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
