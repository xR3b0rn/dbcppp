
#pragma once

#include <dbcppp/SignalMultiplexerValue.h>

namespace dbcppp
{
    class SignalMultiplexerValueImpl
        : public SignalMultiplexerValue
    {
    public:
        SignalMultiplexerValueImpl(
              std::string&& switch_name
            , std::vector<Range>&& value_ranges);
            
        virtual std::unique_ptr<SignalMultiplexerValue> clone() const override;

        virtual std::string getSwitchName() const override;
        virtual void forEachValueRange(std::function<void(const Range&)> cb) const override;

        virtual bool operator==(const SignalMultiplexerValue& rhs) const override;
        virtual bool operator!=(const SignalMultiplexerValue& rhs) const override;

    private:
        std::string _switch_name;
        std::vector<Range> _value_ranges;
    };
}
