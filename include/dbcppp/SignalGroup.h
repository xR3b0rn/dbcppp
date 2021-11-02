
#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "Export.h"
#include "Iterator.h"

namespace dbcppp
{
    class DBCPPP_API ISignalGroup
    {
    public:
        static std::unique_ptr<ISignalGroup> Create(
              uint64_t message_id
            , std::string&& name
            , uint64_t repetitions
            , std::vector<std::string>&& signal_names);

        virtual std::unique_ptr<ISignalGroup> Clone() const = 0;

        virtual ~ISignalGroup() = default;
        virtual uint64_t MessageId() const = 0;
        virtual const std::string& Name() const = 0;
        virtual uint64_t Repetitions() const = 0;
        virtual const std::string& SignalNames_Get(std::size_t i) const = 0;
        virtual uint64_t SignalNames_Size() const = 0;

        DBCPPP_MAKE_ITERABLE(ISignalGroup, SignalNames, std::string);
        
        virtual bool operator==(const ISignalGroup& rhs) const = 0;
        virtual bool operator!=(const ISignalGroup& rhs) const = 0;
    };
}
