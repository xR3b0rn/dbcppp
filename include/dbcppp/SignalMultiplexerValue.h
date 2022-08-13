
#pragma once

#include <string>
#include <cstddef>
#include <memory>
#include <vector>
#include <functional>

#include "Iterator.h"

namespace dbcppp
{
    class ISignalMultiplexerValue
    {
    public:
        struct Range
        {
            std::size_t from;
            std::size_t to;
        };
        
        static std::unique_ptr<ISignalMultiplexerValue> Create(
              std::string&& switch_name
            , std::vector<Range>&& value_ranges);
        
        virtual std::unique_ptr<ISignalMultiplexerValue> Clone() const = 0;

        virtual ~ISignalMultiplexerValue() = default;
        virtual const std::string& SwitchName() const = 0;
        virtual const Range& ValueRanges_Get(std::size_t i) const = 0;
        virtual uint64_t ValueRanges_Size() const = 0;

        DBCPPP_MAKE_ITERABLE(ISignalMultiplexerValue, ValueRanges, Range);

        virtual bool operator==(const ISignalMultiplexerValue& rhs) const = 0;
        virtual bool operator!=(const ISignalMultiplexerValue& rhs) const = 0;
    };
}
