
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
	, std::set<std::string>&& access_nodes
	, std::map<double, std::string>&& value_descriptions
	, uint64_t data_size
	, std::map<std::string, std::unique_ptr<Attribute>>&& attribute_values
	, std::string&& comment)
{
	std::map<std::string, AttributeImpl> avs;
	for (auto& av : attribute_values)
	{
		avs.insert(std::make_pair(av.first, std::move(static_cast<AttributeImpl&>(*av.second))));
		av.second.reset(nullptr);
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
	, std::set<std::string>&& access_nodes
	, std::map<double, std::string>&& value_descriptions
	, uint64_t data_size
	, std::map<std::string, AttributeImpl>&& attribute_values
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
	return _access_nodes.find(name) != _access_nodes.end();
}
std::vector<const std::string*> EnvironmentVariableImpl::getAccessNodes() const
{
	std::vector<const std::string*> result;
	for (auto& n : _access_nodes)
	{
		result.emplace_back(&n);
	}
	return result;
}
const std::string* EnvironmentVariableImpl::getValueDescriptionById(double id) const
{
	const std::string* result = nullptr;
	auto iter = _value_descriptions.find(id);
	if (iter != _value_descriptions.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<double, const std::string*>> EnvironmentVariableImpl::getValueDescriptions() const
{
	std::vector<std::pair<double, const std::string*>> result;
	for (auto& vd : _value_descriptions)
	{
		result.emplace_back(vd.first, &vd.second);
	}
	return result;
}
uint64_t EnvironmentVariableImpl::getDataSize() const
{
	return _data_size;
}
const Attribute* EnvironmentVariableImpl::getAttributeValueByName(const std::string& name) const
{
	const Attribute* result = nullptr;
	auto iter = _attribute_values.find(name);
	if (iter != _attribute_values.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const Attribute*>> EnvironmentVariableImpl::getAttributeValues() const
{
	std::vector<std::pair<std::string, const Attribute*>> result;
	for (const auto& av : _attribute_values)
	{
		result.emplace_back(av.first, &av.second);
	}
	return result;
}
const std::string& EnvironmentVariableImpl::getComment() const
{
	return _comment;
}

void EnvironmentVariable::serializeToStream(std::ostream& os) const
{
	os << "EV_ " << getName() << ": ";
	switch (getVarType())
	{
	case EnvironmentVariable::VarType::Integer: os << "0"; break;
	case EnvironmentVariable::VarType::Float: os << "1"; break;
	case EnvironmentVariable::VarType::String: os << "2"; break;
	case EnvironmentVariable::VarType::Data: os << "0"; break;
	}
	os << " [" << getMinimum() << "|" << getMaximum() << "]" << " \"" << getUnit() << "\" "
		<< getInitialValue() << " " << getEvId() << " ";
	switch (getAccessType())
	{
	case EnvironmentVariable::AccessType::Unrestricted: os << "DUMMY_NODE_VECTOR0"; break;
	case EnvironmentVariable::AccessType::Read: os << "DUMMY_NODE_VECTOR1"; break;
	case EnvironmentVariable::AccessType::Write: os << "DUMMY_NODE_VECTOR2"; break;
	case EnvironmentVariable::AccessType::ReadWrite: os << "DUMMY_NODE_VECTOR3"; break;
	}
	auto access_nodes = getAccessNodes();
	if (access_nodes.size())
	{
		auto iter = access_nodes.begin();
		os << " " << **iter;
		for (iter++; iter != access_nodes.end(); iter++)
		{
			os << ", " << **iter;
		}
	}
	os << ";";
}
