#include <iostream>
#include "SignalTypeImpl.h"

using namespace dbcppp;

std::unique_ptr<ISignalType> ISignalType::Create(
      std::string&& name
    , uint64_t signal_size
    , ISignal::EByteOrder byte_order
    , ISignal::EValueType value_type
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
    , ISignal::EByteOrder byte_order
    , ISignal::EValueType value_type
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
std::unique_ptr<ISignalType> SignalTypeImpl::Clone() const
{
    return std::make_unique<SignalTypeImpl>(*this);
}
const std::string& SignalTypeImpl::Name() const
{
    return _name;
}
uint64_t SignalTypeImpl::SignalSize() const
{
    return _signal_size;
}
ISignal::EByteOrder SignalTypeImpl::ByteOrder() const
{
    return _byte_order;
}
ISignal::EValueType SignalTypeImpl::ValueType() const
{
    return _value_type;
}
double SignalTypeImpl::Factor() const
{
    return _factor;
}
double SignalTypeImpl::Offset() const
{
    return _offset;
}
double SignalTypeImpl::Minimum() const
{
    return _minimum;
}
double SignalTypeImpl::Maximum() const
{
    return _maximum;
}
const std::string& SignalTypeImpl::Unit() const
{
    return _unit;
}
double SignalTypeImpl::DefaultValue() const
{
    return _default_value;
}
const std::string& SignalTypeImpl::ValueTable() const
{
    return _value_table;
}
bool SignalTypeImpl::operator==(const ISignalType& rhs) const
{
    bool equal = true;
    equal &= _name == rhs.Name();
    equal &= _signal_size == rhs.SignalSize();
    equal &= _value_type == rhs.ValueType();
    equal &= _factor == rhs.Factor();
    equal &= _offset == rhs.Offset();
    equal &= _minimum == rhs.Minimum();
    equal &= _maximum == rhs.Maximum();
    equal &= _unit == rhs.Unit();
    equal &= _default_value == rhs.DefaultValue();
    equal &= _value_table == rhs.ValueTable();
    return equal;
}
bool SignalTypeImpl::operator!=(const ISignalType& rhs) const
{
    return !(*this == rhs);
}