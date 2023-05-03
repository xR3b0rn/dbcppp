#pragma once

#include <cstdint>
#include "dbcppp/SignalType.h"

namespace dbcppp
{
    class SignalTypeImpl final
        : public ISignalType
    {
    public:
        SignalTypeImpl(
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
            , std::string&& value_table);

        virtual std::unique_ptr<ISignalType> Clone() const override;

        virtual const std::string& Name() const override;
        virtual uint64_t SignalSize() const override;
        virtual ISignal::EByteOrder ByteOrder() const override;
        virtual ISignal::EValueType ValueType() const override;
        virtual double Factor() const override;
        virtual double Offset() const override;
        virtual double Minimum() const override;
        virtual double Maximum() const override;
        virtual const std::string& Unit() const override;
        virtual double DefaultValue() const override;
        virtual const std::string& ValueTable() const override;
        
        virtual bool operator==(const ISignalType& rhs) const override;
        virtual bool operator!=(const ISignalType& rhs) const override;

    private:
        std::string _name;
        uint64_t _signal_size;
        ISignal::EByteOrder _byte_order;
        ISignal::EValueType _value_type;
        double _factor;
        double _offset;
        double _minimum;
        double _maximum;
        std::string _unit;
        double _default_value;
        std::string _value_table;
    };
}