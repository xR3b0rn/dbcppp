
#pragma once

#include "Export.h"

extern "C"
{
	struct dbcppp_Network;
	DBCPPP_API struct dbcppp_Network* dbcppp_load_from_file(const char* filename);
	DBCPPP_API void free_network(struct dbcppp_network* network);
}
