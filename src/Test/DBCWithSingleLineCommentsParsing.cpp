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


BOOST_AUTO_TEST_CASE(ParsingCppApiWithSingleLineComments)
{
    std::ifstream dbc_file(TEST_DBC_WITH_SINGLE_COMMENTS);

    BOOST_TEST_MESSAGE("Testing dbcppp C and C++ APIs for skipping single line comments...");

    const dbcppp_Network* impl = dbcppp_NetworkLoadDBCFromFile(TEST_DBC_WITH_SINGLE_COMMENTS);
    BOOST_REQUIRE_MESSAGE(impl, "C API DBC parsing failed!");

    auto spec = dbcppp::Network::fromDBC(dbc_file);
    BOOST_REQUIRE_MESSAGE(spec, "C++ API DBC parsing failed!");

    BOOST_REQUIRE_EQUAL(spec->getVersion(), dbcppp_NetworkGetVersion(impl));

    BOOST_TEST_MESSAGE("Done!");
}
