
#include <iostream>
#include "AttributeDefinitionImpl.h"

using namespace dbcppp;

std::unique_ptr<AttributeDefinition> AttributeDefinition::create(
      std::string&& name
    , ObjectType object_type
    , value_type_t&& value_type)
{
    return std::make_unique<AttributeDefinitionImpl>(
          std::move(name)
        , object_type
        , std::move(value_type));
}

AttributeDefinitionImpl::AttributeDefinitionImpl(std::string&& name, ObjectType object_type, value_type_t value_type)
    : _name(std::move(name))
    , _object_type(std::move(object_type))
    , _value_type(std::move(value_type))
{}
std::unique_ptr<AttributeDefinition> AttributeDefinitionImpl::clone() const
{
    return std::make_unique<AttributeDefinitionImpl>(*this);
}
AttributeDefinition::ObjectType AttributeDefinitionImpl::getObjectType() const
{
    return _object_type;
}
const std::string& AttributeDefinitionImpl::getName() const
{
    return _name;
}
const AttributeDefinition::value_type_t& AttributeDefinitionImpl::getValueType() const
{
    return _value_type;
}
void AttributeDefinitionImpl::forEachValueTypeEnum(std::function<void(const std::string&)>&& cb) const
{
    for (const auto& v : boost::get<ValueTypeEnum>(getValueType()).values)
    {
        cb(v);
    }
}
const std::string* AttributeDefinitionImpl::findValueTypeEnum(std::function<bool(const std::string&)>&& pred) const
{
    const std::string* result = nullptr;
    auto begin = boost::get<ValueTypeEnum>(getValueType()).values.cbegin();
    auto end = boost::get<ValueTypeEnum>(getValueType()).values.cend();
    auto iter = std::find_if(begin, end, pred);
    if (iter != end)
    {
        result = &*iter;
    }
    return result;
}

void AttributeDefinition::serializeToStream(std::ostream& os) const
{
    struct VisitorValueType
    {
        VisitorValueType(std::ostream& os)
            : _os(os)
        {}
        void operator()(const AttributeDefinition::ValueTypeInt& vt) const
        {
            _os << " INT" << " " << vt.minimum << " " << vt.maximum;
        }
        void operator()(const AttributeDefinition::ValueTypeHex& vt) const
        {
            _os << " HEX" << " " << vt.minimum << " " << vt.maximum;
        }
        void operator()(const AttributeDefinition::ValueTypeFloat& vt) const
        {
            _os << " FLOAT" << " " << vt.minimum << " " << vt.maximum;
        }
        void operator()(const AttributeDefinition::ValueTypeString& vt) const
        {
            _os << " STRING";
        }
        void operator()(const AttributeDefinition::ValueTypeEnum& vt) const
        {
            _os << " ENUM";
            const auto& values = vt.values;
            if (values.size())
            {
                auto iter = values.begin();
                _os << " \"" << *iter << "\"";
                for (iter++; iter != values.end(); iter++)
                {
                    _os << ", \"" << *iter << "\"";
                }
            }
        }

    private:
        std::ostream& _os;
    };

    std::string object_type = "";
    switch (getObjectType())
    {
    case AttributeDefinition::ObjectType::Network: break;
    case AttributeDefinition::ObjectType::Node: object_type = "BU_"; break;
    case AttributeDefinition::ObjectType::Message: object_type = "BO_"; break;
    case AttributeDefinition::ObjectType::Signal: object_type = "SG_"; break;
    case AttributeDefinition::ObjectType::EnvironmentVariable: object_type = "EV_"; break;
    }
    os << "BA_DEF_ ";
    if (object_type != "")
    {
        os << object_type << " ";
    }
    os << "\"" <<  getName() << "\"";
    boost::apply_visitor(VisitorValueType(os), getValueType());
    os << ";";
}
