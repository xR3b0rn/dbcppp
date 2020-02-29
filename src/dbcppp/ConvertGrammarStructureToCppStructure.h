
#pragma once

#include "NetworkImpl.h"
#include "DBC_Grammar.h"

namespace dbcppp
{
	void ConvertGrammarStructureToCppStructure(const G_Network& gnet, NetworkImpl& net);
}
