#pragma once

#include <vector>
#include <string>
#include <memory>
#include <optional>

#include "Export.h"
#include "SignalType.h"

namespace dbcppp
{
    class DBCPPP_API ValueTable
    {
    public:
        static std::unique_ptr<ValueTable> create(
              std::string&& name
            , std::optional<std::unique_ptr<SignalType>>&& signal_type
            , std::vector<std::tuple<int64_t, std::string>>&& value_encoding_descriptions);
            
        virtual std::unique_ptr<ValueTable> clone() const = 0;

        virtual ~ValueTable() = default;
        virtual const std::string& getName() const = 0;
        virtual std::optional<std::reference_wrapper<const SignalType>> getSignalType() const = 0;
        virtual const std::string* getvalueEncodingDescriptionByValue(int64_t value) const = 0;
        virtual void forEachValueEncodingDescription(std::function<void(int64_t, const std::string&)>&& cb) const = 0;
    };
}