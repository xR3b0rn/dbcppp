
#pragma once

#include <map>
#include <memory>
#include <optional>

#include "../../include/dbcppp/ValueTable.h"
#include "SignalTypeImpl.h"

namespace dbcppp
{
    class ValueTableImpl final
        : public ValueTable
    {
    public:
        ValueTableImpl(
              std::string&& name
            , std::optional<SignalTypeImpl>&& signal_type
            , std::vector<std::tuple<int64_t, std::string>>&& value_encoding_descriptions);

        virtual std::unique_ptr<ValueTable> clone() const override;

        virtual const std::string& getName() const override;
        virtual std::optional<std::reference_wrapper<const SignalType>> getSignalType() const override;
        virtual const std::string* getvalueEncodingDescriptionByValue(int64_t value) const override;
        virtual void forEachValueEncodingDescription(std::function<void(int64_t, const std::string&)>&& cb) const override;
        
        virtual bool operator==(const ValueTable& rhs) const override;
        virtual bool operator!=(const ValueTable& rhs) const override;

    private:
        std::string _name;
        std::optional<SignalTypeImpl> _signal_type;
        std::vector<std::tuple<int64_t, std::string>> _value_encoding_descriptions;
    };
}
