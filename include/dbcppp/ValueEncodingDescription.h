
#pragma once

#include <string>
#include <memory>

#include "Export.h"

namespace dbcppp
{
    class DBCPPP_API IValueEncodingDescription
    {
    public:
        static std::unique_ptr<IValueEncodingDescription> Create(int64_t value, std::string&& description);
        virtual std::unique_ptr<IValueEncodingDescription> Clone() const = 0;
        virtual ~IValueEncodingDescription() = default;

        virtual int64_t Value() const = 0;
        virtual const std::string& Description() const = 0;

        virtual bool operator==(const IValueEncodingDescription& rhs) const = 0;
        virtual bool operator!=(const IValueEncodingDescription& rhs) const = 0;
    };
}
