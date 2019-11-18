
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include <cstddef>

#include "Helper.h"
#include "Node.h"
#include "Attribute.h"

namespace dbcppp
{
	struct EnvironmentVariable
	{
		enum class VarType
		{
			Integer, Float, String, Data
		};
		enum class AccessType
		{
			Unrestricted, Read, Write, ReadWrite
		};
		std::string name;
		VarType var_type;
		double minimum;
		double maximum;
		std::string unit;
		double initial_value;
		uint64_t ev_id;
		AccessType access_type;
		std::set<std::shared_ptr<Node>, SharedNodeCmp> access_nodes;
		std::map<uint64_t, std::string> value_descriptions;
		uint64_t data_size;
		std::map<std::string, Attribute> attribute_values;
		std::string comment;
	};
}
