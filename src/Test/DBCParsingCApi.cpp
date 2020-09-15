
#include <array>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <ctime>
#include <chrono>
#include <random>
#include <string>

#include "../../include/dbcppp/CApi.h"
#include "../../include/dbcppp/Network.h"
#include "Config.h"

#include <boost/test/unit_test.hpp>
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(DBCParsingCApi)
{
    BOOST_TEST_MESSAGE("Testing dbcppp C API for correctness...");

    std::ifstream dbc_file(Core_Lanes_Host_protocol);
    auto spec = dbcppp::Network::fromDBC(dbc_file);
    const dbcppp_Network* impl = dbcppp_NetworkLoadDBCFromFile(Core_Lanes_Host_protocol);
    BOOST_REQUIRE(impl);
    BOOST_REQUIRE_EQUAL(spec->getVersion(), dbcppp_NetworkGetVersion(impl));
    BOOST_REQUIRE_EQUAL(spec->hasNewSymbol("SIGTYPE_VALTYPE_"), dbcppp_NetworkHasNewSymbol(impl, "SIGTYPE_VALTYPE_"));
    BOOST_REQUIRE_EQUAL(!spec->hasNewSymbol("Not there"), !dbcppp_NetworkHasNewSymbol(impl, "Not there"));

    dbcppp_NetworkFree(impl);
}