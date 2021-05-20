
#pragma once

#include <string>
#include <cstddef>
#include <memory>
#include <vector>
#include <functional>

namespace dbcppp
{
    class SignalMultiplexerValue
    {
    public:
        struct Range
        {
            std::size_t from;
            std::size_t to;
        };
        
        static std::unique_ptr<SignalMultiplexerValue> create(
              std::string&& switch_name
            , std::vector<Range>&& value_ranges);
        
        virtual std::unique_ptr<SignalMultiplexerValue> clone() const = 0;

        virtual ~SignalMultiplexerValue() = default;
        virtual std::string getSwitchName() const = 0;
        virtual void forEachValueRange(std::function<void(const Range&)> cb) const = 0;

        virtual bool operator==(const SignalMultiplexerValue& rhs) const = 0;
        virtual bool operator!=(const SignalMultiplexerValue& rhs) const = 0;
    };
}
