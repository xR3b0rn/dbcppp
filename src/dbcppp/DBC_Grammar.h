
#pragma once

#include <istream>
#include <Vector/DBC/dbcppp_export.h>
#include "Network.h"

DBCPPP_EXPORT bool operator>>(std::istream& is, dbcppp::Network& net);
