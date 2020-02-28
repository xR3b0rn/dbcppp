
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include <cstddef>

#include "Export.h"
#include "Helper.h"
#include "Node.h"
#include "Attribute.h"

namespace dbcppp
{
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
		virtual const std::string* getValueDescriptionById(uint64_t id) const = 0;
		virtual std::vector<std::pair<uint64_t, const std::string*>> getValueDescriptions() const = 0;
		virtual uint64_t getDataSize() const = 0;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const = 0;
		virtual const std::string& getComment() const = 0;
	};
}
