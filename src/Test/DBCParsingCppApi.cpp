
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

#include "../../include/dbcppp/Network2DBC.h"
#include "../../include/dbcppp/CApi.h"
#include "../../include/dbcppp/Network.h"
#include "Config.h"

#include <boost/test/unit_test.hpp>
namespace utf = boost::unit_test;

std::vector<std::string> dbc_to_vec(std::istream& is)
{
    std::vector<std::string> result;
    std::string line;
    while (std::getline(is, line))
    {
        if (line != "")
        {
            result.push_back(line);
        }
    }
    return result;
}

BOOST_AUTO_TEST_CASE(ParsingCppApi)
{
    std::string dbc_file(TEST_DBC);

    BOOST_TEST_MESSAGE("Testing dbcppp C++ API for correctness...");

    if (dbc_file != "")
    {
        std::vector<std::string> spec;
        {
            std::ifstream idbc(dbc_file);
            spec = dbc_to_vec(idbc);
        }
        std::ifstream idbc(dbc_file);
        auto net = dbcppp::Network::fromDBC(idbc);
        std::clock_t begin = std::clock();

        BOOST_REQUIRE_MESSAGE(net, "DBC parsing failed!");

        std::stringstream ss;
        ss << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
        {
            using namespace dbcppp::Network2DBC;
            ss << *net;
        }
        auto imp = dbc_to_vec(ss);

        for (const auto& line : imp)
        {
            auto iter = std::find(spec.begin(), spec.end(), line);
            BOOST_CHECK_MESSAGE(iter != spec.end(), "line: \"" << line << "\" doesn't match any line in the DBC file!");
            if (iter != spec.end())
            {
                spec.erase(iter);
            }
        }
        {
            std::stringstream ss;
            for (const auto& line : spec)
            {
                ss << line << "\n";
            }
            BOOST_CHECK_MESSAGE(spec.empty(), "Spec isn't empty!\nNot found lines:\n" << ss.str());
        }
    }
    BOOST_TEST_MESSAGE("Done!");
}
