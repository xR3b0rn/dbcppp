
#pragma once

#include "../../include/dbcppp/EnvironmentVariable.h"
#include "NodeImpl.h"
#include "AttributeImpl.h"

namespace dbcppp
{
	class EnvironmentVariableImpl final
		: public EnvironmentVariable
	{
	public:
		EnvironmentVariableImpl(
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
			, std::string&& comment);
		EnvironmentVariableImpl(EnvironmentVariableImpl&&) = default;
		EnvironmentVariableImpl& operator=(EnvironmentVariableImpl&&);

		virtual const std::string& getName() const override;
		virtual VarType getVarType() const override;
		virtual double getMinimum() const override;
		virtual double getMaximum() const override;
		virtual std::string getUnit() const override;
		virtual double getInitialValue() const override;
		virtual uint64_t getEvId() const override;
		virtual AccessType getAccessType() const override;
		virtual bool hasAccessNode(const std::string& name) const override;
		virtual std::vector<const std::string*> getAccessNodes() const override;
		virtual const std::string* getValueDescriptionById(double id) const override;
		virtual std::vector<std::pair<double, const std::string*>> getValueDescriptions() const override;
		virtual uint64_t getDataSize() const override;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const override;
		virtual const std::string& getComment() const override;

		std::string _name;
		VarType _var_type;
		double _minimum;
		double _maximum;
		std::string _unit;
		double _initial_value;
		uint64_t _ev_id;
		AccessType _access_type;
		std::set<std::string> _access_nodes;
		std::map<double, std::string> _value_descriptions;
		uint64_t _data_size;
		std::map<std::string, AttributeImpl> _attribute_values;
		std::string _comment;
	};
}
