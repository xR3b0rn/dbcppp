
#include "AttributeDefinitionImpl.h"

using namespace dbcppp;

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
