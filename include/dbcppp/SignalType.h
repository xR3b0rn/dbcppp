
#pragma once

#include <cstddef>
#include <string>
#include <memory>

#include "Export.h"
#include "Signal.h"

namespace dbcppp
{
    class SignalType
    {
    public:
        static std::unique_ptr<SignalType> create(
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
            , std::string&& value_table);
            
        virtual std::unique_ptr<SignalType> clone() const = 0;

        virtual ~SignalType() = default;
        virtual const std::string& getName() const = 0;
        virtual uint64_t getSignalSize() const = 0;
        virtual Signal::ByteOrder getByteOrder() const = 0;
        virtual Signal::ValueType getValueType() const = 0;
        virtual double getFactor() const = 0;
        virtual double getOffset() const = 0;
        virtual double getMinimum() const = 0;
        virtual double getMaximum() const = 0;
        virtual const std::string& getUnit() const = 0;
        virtual double getDefaultValue() const = 0;
        virtual const std::string& getValueTable() const = 0;
        
        void serializeToStream(std::ostream& os) const;
    };
}
