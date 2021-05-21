#pragma once

#include <cstddef>
#include <string>
#include <memory>

#include "Export.h"
#include "Signal.h"

namespace dbcppp
{
    class ISignalType
    {
    public:
        static std::unique_ptr<ISignalType> Create(
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
            
        virtual std::unique_ptr<ISignalType> Clone() const = 0;

        virtual ~ISignalType() = default;
        virtual const std::string& Name() const = 0;
        virtual uint64_t SignalSize() const = 0;
        virtual ISignal::EByteOrder ByteOrder() const = 0;
        virtual ISignal::EValueType ValueType() const = 0;
        virtual double Factor() const = 0;
        virtual double Offset() const = 0;
        virtual double Minimum() const = 0;
        virtual double Maximum() const = 0;
        virtual const std::string& Unit() const = 0;
        virtual double DefaultValue() const = 0;
        virtual const std::string& ValueTable() const = 0;
        
        virtual bool operator==(const ISignalType& rhs) const = 0;
        virtual bool operator!=(const ISignalType& rhs) const = 0;
    };
}