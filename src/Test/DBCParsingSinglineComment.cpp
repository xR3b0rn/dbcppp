
#include <array>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <ctime>
#include <chrono>
#include <random>
#include <string>
#include <iomanip>

#include "../../include/dbcppp/Network2Functions.h"
#include "../../include/dbcppp/CApi.h"
#include "../../include/dbcppp/Network.h"
#include "Config.h"

#include <boost/test/unit_test.hpp>
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(IgnoringSinglelineComments)
{
    std::string dbc_file(Core_Lanes_Host_protocol);

    BOOST_TEST_MESSAGE("Testing dbcppp C++ API for Single line comments");

    if (dbc_file != "")
    {
        std::ifstream idbc(dbc_file);
        const auto net = dbcppp::Network::fromDBC(idbc);
		const dbcppp_Network* impl = dbcppp_NetworkLoadDBCFromFile(Core_Lanes_Host_protocol);
        BOOST_REQUIRE_MESSAGE(net, "DBC c++ parsing failed!");
		BOOST_REQUIRE_MESSAGE(impl, "DBC c parsing failed!");
    }
    BOOST_TEST_MESSAGE("Done!");
}
