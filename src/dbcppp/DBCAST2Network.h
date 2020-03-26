
#pragma once

#include "NetworkImpl.h"
#include "DBC_Grammar.h"

namespace dbcppp
{
    std::unique_ptr<Network> DBCAST2Network(const G_Network& gnet);
}
