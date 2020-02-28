
#pragma once

#include "NetworkImpl.h"
#include "DBC_Grammar.h"

namespace dbcppp
{
	void ConvertGrammarStructorToCppStructure(const G_Network& gnet, NetworkImpl& net);
}
