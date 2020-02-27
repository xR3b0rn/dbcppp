
#include "SignalTypeImpl.h"

using namespace dbcppp;

const std::string& SignalTypeImpl::getName() const
{
	return _name;
}
uint64_t SignalTypeImpl::getSignalSize() const
{
	return _signal_size;
}
Signal::ByteOrder SignalTypeImpl::getByteOrder() const
{
	return _byte_order;
}
Signal::ValueType SignalTypeImpl::getValueType() const
{
	return _value_type;
}
double SignalTypeImpl::getFactor() const
{
	return _factor;
}
double SignalTypeImpl::getOffset() const
{
	return _offset;
}
double SignalTypeImpl::getMinimum() const
{
	return _minimum;
}
double SignalTypeImpl::getMaximum() const
{
	return _maximum;
}
const std::string& SignalTypeImpl::getUnit() const
{
	return _unit;
}
double SignalTypeImpl::getDefaultValue() const
{
	return _default_value;
}
const std::string& SignalTypeImpl::getValueTable() const
{
	return _value_table;
}