
#pragma once

#include "NetworkImpl.h"
#include "DBC_Grammar.h"

namespace dbcppp
{
	NetworkImpl ConvertGrammarStructureToCppStructure(const G_Network& gnet);
}
