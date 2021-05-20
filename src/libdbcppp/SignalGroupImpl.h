
#pragma once

#include <dbcppp/SignalGroup.h>

namespace dbcppp
{
    class SignalGroupImpl
        : public SignalGroup
    {
    public:
        virtual std::unique_ptr<SignalGroup> clone() const override;

        SignalGroupImpl(
              uint64_t message_id
            , std::string&& name
            , uint64_t repetitions
            , std::vector<std::string>&& signal_names);
        virtual uint64_t getMessageId() const override;
        virtual const std::string& getName() const override;
        virtual uint64_t getRepetitions() const override;
        virtual void forEachSignalName(std::function<void(const std::string&)> cb) const override;
        
        virtual bool operator==(const SignalGroup& rhs) const override;
        virtual bool operator!=(const SignalGroup& rhs) const override;

    private:
        uint64_t _message_id;
        std::string _name;
        uint64_t _repetitions;
        std::vector<std::string> _signal_names;
    };
}
