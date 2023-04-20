
#pragma once

#include <dbcppp/SignalMultiplexerValue.h>

namespace dbcppp
{
    class SignalMultiplexerValueImpl
        : public ISignalMultiplexerValue
    {
    public:
        SignalMultiplexerValueImpl(
              std::string&& switch_name
            , std::vector<Range>&& value_ranges);
            
        virtual std::unique_ptr<ISignalMultiplexerValue> Clone() const override;

        virtual const std::string& SwitchName() const override;
        virtual const Range& ValueRanges_Get(std::size_t i) const override;
        virtual uint64_t ValueRanges_Size() const override;

        virtual bool operator==(const ISignalMultiplexerValue& rhs) const override;
        virtual bool operator!=(const ISignalMultiplexerValue& rhs) const override;

    private:
        std::string _switch_name;
        std::vector<Range> _value_ranges;
    };
}
