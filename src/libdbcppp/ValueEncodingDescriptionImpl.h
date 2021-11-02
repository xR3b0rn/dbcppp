
#pragma once

#include <string>
#include <dbcppp/ValueEncodingDescription.h>

namespace dbcppp
{
    class ValueEncodingDescriptionImpl
        : public IValueEncodingDescription
    {
    public:
        virtual std::unique_ptr<IValueEncodingDescription> Clone() const override;

        ValueEncodingDescriptionImpl(int64_t value, std::string&& description);
        virtual int64_t Value() const override;
        virtual const std::string& Description() const override;
        
        virtual bool operator==(const IValueEncodingDescription& rhs) const override;
        virtual bool operator!=(const IValueEncodingDescription& rhs) const override;

    private:
        int64_t _value;
        std::string _description;
    };
}
