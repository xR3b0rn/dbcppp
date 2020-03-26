
#include "../../include/dbcppp/Network.h"
#include "ValueTableImpl.h"

using namespace dbcppp;

std::unique_ptr<ValueTable> ValueTable::create(
      std::string&& name
    , boost::optional<std::unique_ptr<SignalType>>&& signal_type
    , std::map<double, std::string>&& value_encoding_descriptions)
{
    boost::optional<SignalTypeImpl> st;
    if (signal_type)
    {
        st = std::move(static_cast<SignalTypeImpl&>(**signal_type));
        (*signal_type).reset(nullptr);
    }
    return std::make_unique<ValueTableImpl>(std::move(name), std::move(st), std::move(value_encoding_descriptions));
}
ValueTableImpl::ValueTableImpl(
      std::string&& name
    , boost::optional<SignalTypeImpl>&& signal_type
    , std::map<double, std::string>&& value_encoding_descriptions)

    : _name(std::move(name))
    , _signal_type(std::move(signal_type))
    , _value_encoding_descriptions(std::move(value_encoding_descriptions))
{}
std::unique_ptr<ValueTable> ValueTableImpl::clone() const
{
    return std::make_unique<ValueTableImpl>(*this);
}
const std::string& ValueTableImpl::getName() const
{
    return _name;
}
boost::optional<const SignalType&> ValueTableImpl::getSignalType() const
{
    if (_signal_type)
    {
        return *_signal_type;
    }
    return boost::none;
}
void ValueTableImpl::forEachValueEncodingDescription(std::function<void(double, const std::string&)>&& cb) const
{
    for (const auto& ved : _value_encoding_descriptions)
    {
        cb(ved.first, ved.second);
    }
}

void ValueTable::serializeToStream(std::ostream& os) const
{
    bool first = true;
    forEachValueEncodingDescription(
        [&](double value, const std::string& desc)
        {
            if (first)
            {
                first = false;
                os << "VAL_TABLE_ " << getName();
            }
            os << " " << value << " \"" << desc << "\"";
        });
    if (!first)
    {
        os << ";";
    }
}
