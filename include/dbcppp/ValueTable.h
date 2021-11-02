#pragma once

#include <vector>
#include <string>
#include <memory>
#include <optional>

#include "Export.h"
#include "SignalType.h"
#include "ValueEncodingDescription.h"

namespace dbcppp
{
    class DBCPPP_API IValueTable
    {
    public:
        static std::unique_ptr<IValueTable> Create(
              std::string&& name
            , std::optional<std::unique_ptr<ISignalType>>&& signal_type
            , std::vector<std::unique_ptr<IValueEncodingDescription>>&& value_encoding_descriptions);
            
        virtual std::unique_ptr<IValueTable> Clone() const = 0;

        virtual ~IValueTable() = default;
        virtual const std::string& Name() const = 0;
        virtual std::optional<std::reference_wrapper<const ISignalType>> SignalType() const = 0;
        virtual const IValueEncodingDescription& ValueEncodingDescriptions_Get(std::size_t i) const = 0;
        virtual uint64_t ValueEncodingDescriptions_Size() const = 0;

        DBCPPP_MAKE_ITERABLE(IValueTable, ValueEncodingDescriptions, IValueEncodingDescription);
        
        virtual bool operator==(const IValueTable& rhs) const = 0;
        virtual bool operator!=(const IValueTable& rhs) const = 0;
    };
}