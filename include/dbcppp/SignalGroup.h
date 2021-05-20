
#pragma once

#include <vector>
#include <string>
#include <functional>

#include "Export.h"

namespace dbcppp
{
    class DBCPPP_API SignalGroup
    {
    public:
        static std::unique_ptr<SignalGroup> create(
              uint64_t message_id
            , std::string&& name
            , uint64_t repetitions
            , std::vector<std::string>&& signal_names);

        virtual std::unique_ptr<SignalGroup> clone() const = 0;

        virtual ~SignalGroup() = default;
        virtual uint64_t getMessageId() const = 0;
        virtual const std::string& getName() const = 0;
        virtual uint64_t getRepetitions() const = 0;
        virtual void forEachSignalName(std::function<void(const std::string&)> cb) const = 0;
        
        virtual bool operator==(const SignalGroup& rhs) const = 0;
        virtual bool operator!=(const SignalGroup& rhs) const = 0;
    };
}
