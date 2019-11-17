
#pragma once

#include <map>
#include <set>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "BitTiming.h"
#include "ValueTable.h"
#include "Node.h"
#include "Message.h"
#include "EnvironmentVariable.h"
#include "SignalType.h"
#include "AttributeDefinition.h"
#include "Attribute.h"

namespace dbcppp
{
	struct Network
	{
		std::string version;
		std::vector<std::string> new_symbols;
		BitTiming bit_timing;
		std::map<std::string, std::shared_ptr<Node>> nodes;
		std::map<std::string, ValueTable> value_tables;
		std::unordered_map<uint64_t, std::shared_ptr<Message>> messages;
		std::map<std::string, EnvironmentVariable> environment_variables;
		std::map<std::string, SignalType> signal_types;
		std::map<std::string, AttributeDefinition> attribute_definitions;
		std::map<std::string, Attribute> attribute_defaults;
		std::map<std::string, Attribute> attribute_values;
		//std::map<std::string, AttributeRelation> attribute_relation_values;
		std::string comment;
	};
}
#ifdef _WIN32
#   define DBCPPP_EXPORT  __declspec( dllexport )
#else
#   define DBCPPP_EXPORT
#endif
DBCPPP_EXPORT bool operator>>(std::istream& is, dbcppp::Network& net);
