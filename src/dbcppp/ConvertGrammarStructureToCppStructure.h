
#pragma once

#include "NetworkImpl.h"
#include "DBC_Grammar.h"

namespace dbcppp
{
	std::unique_ptr<Network> ConvertGrammarStructureToCppStructure(const G_Network& gnet);
}
