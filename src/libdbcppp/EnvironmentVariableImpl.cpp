#include "EnvironmentVariableImpl.h"

using namespace dbcppp;

std::unique_ptr<EnvironmentVariable> EnvironmentVariable::create(
      std::string&& name
    , VarType var_type
    , double minimum
    , double maximum
    , std::string&& unit
    , double initial_value
    , uint64_t ev_id
    , AccessType access_type
    , std::vector<std::string>&& access_nodes
    , std::vector<std::tuple<int64_t, std::string>>&& value_descriptions
    , uint64_t data_size
    , std::vector<std::unique_ptr<Attribute>>&& attribute_values
    , std::string&& comment)
{
    std::vector<AttributeImpl> avs;
    for (auto& av : attribute_values)
    {
        avs.push_back(std::move(static_cast<AttributeImpl&>(*av)));
        av.reset(nullptr);
    }
    return std::make_unique<EnvironmentVariableImpl>(
          std::move(name)
        , var_type
        , minimum
        , maximum
        , std::move(unit)
        , initial_value
        , ev_id
        , access_type
        , std::move(access_nodes)
        , std::move(value_descriptions)
        , data_size
        , std::move(avs)
        , std::move(comment));
}

EnvironmentVariableImpl::EnvironmentVariableImpl(
      std::string&& name
    , VarType var_type
    , double minimum
    , double maximum
    , std::string&& unit
    , double initial_value
    , uint64_t ev_id
    , AccessType access_type
    , std::vector<std::string>&& access_nodes
    , std::vector<std::tuple<int64_t, std::string>>&& value_descriptions
    , uint64_t data_size
    , std::vector<AttributeImpl>&& attribute_values
    , std::string&& comment)
    
    : _name(std::move(name))
    , _var_type(std::move(var_type))
    , _minimum(std::move(minimum))
    , _maximum(std::move(maximum))
    , _unit(std::move(unit))
    , _initial_value(std::move(initial_value))
    , _ev_id(std::move(ev_id))
    , _access_type(std::move(access_type))
    , _access_nodes(std::move(access_nodes))
    , _value_descriptions(std::move(value_descriptions))
    , _data_size(std::move(data_size))
    , _attribute_values(std::move(attribute_values))
    , _comment(std::move(comment))
{}
std::unique_ptr<EnvironmentVariable> EnvironmentVariableImpl::clone() const
{
    return std::make_unique<EnvironmentVariableImpl>(*this);
}
const std::string& EnvironmentVariableImpl::getName() const
{
    return _name;
}
EnvironmentVariable::VarType EnvironmentVariableImpl::getVarType() const
{
    return _var_type;
}
double EnvironmentVariableImpl::getMinimum() const
{
    return _minimum;
}
double EnvironmentVariableImpl::getMaximum() const
{
    return _maximum;
}
std::string EnvironmentVariableImpl::getUnit() const
{
    return _unit;
}
double EnvironmentVariableImpl::getInitialValue() const
{
    return _initial_value;
}
uint64_t EnvironmentVariableImpl::getEvId() const
{
    return _ev_id;
}
EnvironmentVariable::AccessType EnvironmentVariableImpl::getAccessType() const
{
    return _access_type;
}
bool EnvironmentVariableImpl::hasAccessNode(const std::string& name) const
{
    auto iter = std::find(_access_nodes.begin(), _access_nodes.end(), name);
    return iter != _access_nodes.end();
}
void EnvironmentVariableImpl::forEachAccessNode(std::function<void(const std::string&)> cb) const
{
    for (const auto& n : _access_nodes)
    {
        cb(n);
    }
}
const std::string* EnvironmentVariableImpl::getValueDescriptionByValue(int64_t value) const
{
    const std::string* result = nullptr;
    auto iter = std::find_if(_value_descriptions.begin(), _value_descriptions.end(),
      [&](const auto& vd) { return std::get<0>(vd) == value;});
    if (iter != _value_descriptions.end())
    {
        result = &std::get<1>(*iter);
    }
    return result;
}
void EnvironmentVariableImpl::forEachValueDescription(std::function<void(int64_t, const std::string&)> cb) const
{
    for (const auto& vd : _value_descriptions)
    {
        cb(std::get<0>(vd), std::get<1>(vd));
    }
}
uint64_t EnvironmentVariableImpl::getDataSize() const
{
    return _data_size;
}
const Attribute* EnvironmentVariableImpl::getAttributeValueByName(const std::string& name) const
{
    const Attribute* result = nullptr;
    auto iter = std::find_if(_attribute_values.begin(), _attribute_values.end(),
        [&](const AttributeImpl& av) { return av.getName() == name; });
    if (iter != _attribute_values.end())
    {
        result = &*iter;
    }
    return result;
}
const Attribute* EnvironmentVariableImpl::findAttributeValue(std::function<bool(const Attribute&)> pred) const
{
    const Attribute* result = nullptr;
    auto iter = std::find_if(_attribute_values.begin(), _attribute_values.end(), pred);
    if (iter != _attribute_values.end())
    {
        result = &*iter;
    }
    return result;
}
void EnvironmentVariableImpl::forEachAttributeValue(std::function<void(const Attribute&)> cb) const
{
    for (const auto& av : _attribute_values)
    {
        cb(av);
    }
}
const std::string& EnvironmentVariableImpl::getComment() const
{
    return _comment;
}
bool EnvironmentVariableImpl::operator==(const EnvironmentVariable& rhs) const
{
    bool result = true;
    result &= _name == rhs.getName();
    result &= _var_type == rhs.getVarType();
    result &= _minimum == rhs.getMinimum();
    result &= _unit == rhs.getUnit();
    result &= _initial_value == rhs.getInitialValue();
    result &= _ev_id == rhs.getEvId();
    result &= _access_type == rhs.getAccessType();
    rhs.forEachAccessNode(
        [&](const std::string& node)
        {
            auto beg = _access_nodes.begin();
            auto end = _access_nodes.end();
            result &= std::find(beg, end, node) != end;
        });
    rhs.forEachValueDescription(
        [&](int64_t value, const std::string& desc)
        {
            auto beg = _value_descriptions.begin();
            auto end = _value_descriptions.end();
            result &= std::find(beg, end, std::make_tuple(value, desc)) != end;
        });
    result &= _data_size == rhs.getDataSize();
    rhs.forEachAttributeValue(
        [&](const dbcppp::Attribute& attr)
        {
            auto beg = _attribute_values.begin();
            auto end = _attribute_values.end();
            result &= std::find(beg, end, attr) != end;
        });
    result &= _comment == rhs.getComment();
    return result;
}
bool EnvironmentVariableImpl::operator!=(const EnvironmentVariable& rhs) const
{
    return !(*this == rhs);
}