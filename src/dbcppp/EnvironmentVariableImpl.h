
#pragma once

#include "EnvironmentVariable.h"
#include "NodeImpl.h"
#include "AttributeImpl.h"

namespace dbcppp
{
	class EnvironmentVariableImpl
		: public EnvironmentVariable
	{
	public:
		virtual const std::string& getName() const override;
		virtual VarType getVarType() const override;
		virtual double getMinimum() const override;
		virtual double getMaximum() const override;
		virtual std::string getUnit() const override;
		virtual double getInitialValue() const override;
		virtual uint64_t getEvId() const override;
		virtual AccessType getAccessType() const override;
		virtual const Node* getAccessNodeByName(const std::string& name) const override;
		virtual std::vector<const Node*> getAccessNodes() const override;
		virtual const std::string* getValueDescriptionById(uint64_t id) const override;
		virtual std::vector<std::pair<uint64_t, const std::string*>> getValueDescriptions() const override;
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
		std::set<NodeImpl*> _access_nodes;
		std::map<uint64_t, std::string> _value_descriptions;
		uint64_t _data_size;
		std::map<std::string, AttributeImpl> _attribute_values;
		std::string _comment;
	};
}
