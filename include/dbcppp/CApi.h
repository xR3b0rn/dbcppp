
#pragma once

extern "C"
{
#include "Export.h"

	struct dbcppp_Network;
	DBCPPP_API struct dbcppp_Network* dbcppp_load_from_file(const char* filename);
	DBCPPP_API void free_network(struct dbcppp_Network* net);

	const char* dbcppp_network_get_version(struct dbcppp_Network* net);
	struct dbcppp_Node;
	void dbcppp_network_for_each_node(struct dbcppp_Network* net, void(*cb)(const char*, dbcppp_Node*));
}
