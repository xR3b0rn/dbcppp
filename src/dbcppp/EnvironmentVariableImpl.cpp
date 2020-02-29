
#include "EnvironmentVariableImpl.h"

using namespace dbcppp;

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
const std::string* EnvironmentVariableImpl::getValueDescriptionById(uint64_t id) const
{
	const std::string* result = nullptr;
	auto iter = _value_descriptions.find(id);
	if (iter != _value_descriptions.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<uint64_t, const std::string*>> EnvironmentVariableImpl::getValueDescriptions() const
{
	std::vector<std::pair<uint64_t, const std::string*>> result;
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
	for (auto& av : _attribute_values)
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
	for (const auto& n : getAccessNodes())
	{
		os << " " << *n;
	}
	os << ";";
}
