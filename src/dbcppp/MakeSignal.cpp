
#include "MakeSignal.h"
#include "../dbcppp/Network.h"

using namespace dbcppp;



std::shared_ptr<dbcppp::Signal> dbcppp::make_signal(Signal::ByteOrder byte_order, Signal::ValueType value_type)
{
	auto sig = std::make_shared<Signal>();
	else
	{
		// can't be reached
		throw std::runtime_error("Couldn't determine Signal!");
	}
	return sig;
}
