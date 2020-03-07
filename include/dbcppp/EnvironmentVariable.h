
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include <cstddef>

#include "Export.h"
#include "Node.h"
#include "Attribute.h"

namespace dbcppp
{
	class Network;
	class DBCPPP_API EnvironmentVariable
	{
	public:
		enum class VarType
		{
			Integer, Float, String, Data
		};
		enum class AccessType
		{
			Unrestricted, Read, Write, ReadWrite
		};

		static std::unique_ptr<EnvironmentVariable> create(
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
			, std::string&& comment);

		virtual ~EnvironmentVariable() = default;
		virtual const std::string& getName() const = 0;
		virtual VarType getVarType() const = 0;
		virtual double getMinimum() const = 0;
		virtual double getMaximum() const = 0;
		virtual std::string getUnit() const = 0;
		virtual double getInitialValue() const = 0;
		virtual uint64_t getEvId() const = 0;
		virtual AccessType getAccessType() const = 0;
		virtual bool hasAccessNode(const std::string& name) const = 0;
		virtual std::vector<const std::string*> getAccessNodes() const = 0;
		virtual const std::string* getValueDescriptionById(double id) const = 0;
		virtual std::vector<std::pair<double, const std::string*>> getValueDescriptions() const = 0;
		virtual uint64_t getDataSize() const = 0;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const = 0;
		virtual const std::string& getComment() const = 0;
		
		void serializeToStream(std::ostream& os) const;
	};
}
