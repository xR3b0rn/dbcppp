
#pragma once

#include <dbcppp/SignalGroup.h>

namespace dbcppp
{
    class SignalGroupImpl
        : public ISignalGroup
    {
    public:
        virtual std::unique_ptr<ISignalGroup> Clone() const override;

        SignalGroupImpl(
              uint64_t message_id
            , std::string&& name
            , uint64_t repetitions
            , std::vector<std::string>&& signal_names);
        virtual uint64_t MessageId() const override;
        virtual const std::string& Name() const override;
        virtual uint64_t Repetitions() const override;
        virtual const std::string& SignalNames_Get(std::size_t i) const override;
        virtual uint64_t SignalNames_Size() const override;
        
        virtual bool operator==(const ISignalGroup& rhs) const override;
        virtual bool operator!=(const ISignalGroup& rhs) const override;

    private:
        uint64_t _message_id;
        std::string _name;
        uint64_t _repetitions;
        std::vector<std::string> _signal_names;
    };
}
