
#pragma once

#include <cstddef>
#include <string>
#include <memory>

#include "Export.h"
#include "Helper.h"
#include "Node.h"
#include "Signal.h"
#include "Attribute.h"

namespace dbcppp
{
	struct DBCPPP_EXPORT Message
	{
		uint64_t id;
		std::string name;
		uint64_t message_size;
		std::shared_ptr<Node> transmitter;
		std::set<std::shared_ptr<Node>, SharedNodeCmp> transmitters;
		std::map<std::string, std::shared_ptr<Signal>> signals;
		std::map<std::string, Attribute> attribute_values;
		std::string comment;
	};
}
