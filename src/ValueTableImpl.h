#pragma once

#include <map>
#include <memory>
#include <optional>
#include <functional>

#include "dbcppp/ValueTable.h"
#include "SignalTypeImpl.h"
#include "ValueEncodingDescriptionImpl.h"

namespace dbcppp
{
    class SignalType;
    class ValueTableImpl final
        : public IValueTable
    {
    public:
        ValueTableImpl(
              std::string&& name
            , std::optional<SignalTypeImpl>&& signal_type
            , std::vector<ValueEncodingDescriptionImpl>&& value_encoding_descriptions);

        virtual std::unique_ptr<IValueTable> Clone() const override;

        virtual const std::string& Name() const override;
        virtual std::optional<std::reference_wrapper<const ISignalType>> SignalType() const override;
        virtual const IValueEncodingDescription& ValueEncodingDescriptions_Get(std::size_t i) const override;
        virtual uint64_t ValueEncodingDescriptions_Size() const override;
        
        virtual bool operator==(const IValueTable& rhs) const override;
        virtual bool operator!=(const IValueTable& rhs) const override;

    private:
        std::string _name;
        std::optional<SignalTypeImpl> _signal_type;
        std::vector<ValueEncodingDescriptionImpl> _value_encoding_descriptions;
    };
}