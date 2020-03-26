
#include <iostream>
#include "SignalTypeImpl.h"

using namespace dbcppp;

std::unique_ptr<SignalType> SignalType::create(
      std::string&& name
    , uint64_t signal_size
    , Signal::ByteOrder byte_order
    , Signal::ValueType value_type
    , double factor
    , double offset
    , double minimum
    , double maximum
    , std::string&& unit
    , double default_value
    , std::string&& value_table)
{
    return std::make_unique<SignalTypeImpl>(
          std::move(name)
        , signal_size
        , byte_order
        , value_type
        , factor
        , offset
        , minimum
        , maximum
        , std::move(unit)
        , default_value
        , std::move(value_table));
}

SignalTypeImpl::SignalTypeImpl(
      std::string&& name
    , uint64_t signal_size
    , Signal::ByteOrder byte_order
    , Signal::ValueType value_type
    , double factor
    , double offset
    , double minimum
    , double maximum
    , std::string&& unit
    , double default_value
    , std::string&& value_table)

    : _name(std::move(name))
    , _signal_size(std::move(signal_size))
    , _byte_order(std::move(byte_order))
    , _value_type(std::move(value_type))
    , _factor(std::move(factor))
    , _offset(std::move(offset))
    , _minimum(std::move(minimum))
    , _maximum(std::move(maximum))
    , _unit(std::move(unit))
    , _default_value(std::move(default_value))
    , _value_table(std::move(value_table))
{}
std::unique_ptr<SignalType> SignalTypeImpl::clone() const
{
    return std::make_unique<SignalTypeImpl>(*this);
}
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

void SignalType::serializeToStream(std::ostream& os) const
{
    os << "SGTYPE_ " << getName() << " : " << getSignalSize() << "@";
    switch (getByteOrder())
    {
    case Signal::ByteOrder::BigEndian: os << "0"; break;
    case Signal::ByteOrder::LittleEndian: os << "1"; break;
    }
    switch (getValueType())
    {
    case Signal::ValueType::Unsigned: os << "+ "; break;
    case Signal::ValueType::Signed: os << "- "; break;
    }
    os << "(" << getFactor() << "," << getOffset() << ") ";
    os << "[" << getMinimum() << "|" << getMaximum() << "] ";
    os << "\"" << getUnit() << "\" " << getDefaultValue();
    os << ", " << getValueTable();
    os << ";";
}
