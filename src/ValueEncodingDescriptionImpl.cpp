
#include "ValueEncodingDescriptionImpl.h"

using namespace dbcppp;

std::unique_ptr<IValueEncodingDescription> IValueEncodingDescription::Create(int64_t value, std::string&& description)
{
    return std::make_unique<ValueEncodingDescriptionImpl>(value, std::move(description));
}

std::unique_ptr<IValueEncodingDescription> ValueEncodingDescriptionImpl::Clone() const
{
    return std::make_unique<ValueEncodingDescriptionImpl>(*this);
}

ValueEncodingDescriptionImpl::ValueEncodingDescriptionImpl(int64_t value, std::string&& description)
    : _value(value)
    , _description(std::move(description))
{}
int64_t ValueEncodingDescriptionImpl::Value() const
{
    return _value;
}
const std::string& ValueEncodingDescriptionImpl::Description() const
{
    return _description;
}

bool ValueEncodingDescriptionImpl::operator==(const IValueEncodingDescription& rhs) const
{
    bool equal = true;
    equal &= _value == rhs.Value();
    equal &= _description == rhs.Description();
    return equal;
}
bool ValueEncodingDescriptionImpl::operator!=(const IValueEncodingDescription& rhs) const
{
    return !(*this == rhs);
}
