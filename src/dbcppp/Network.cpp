
#include "Network.h"
#include "NetworkImpl.h"

using namespace dbcppp;


std::unique_ptr<Network> Network::create()
{
	return std::make_unique<NetworkImpl>();
}
