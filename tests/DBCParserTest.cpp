
#include <fstream>
#include <iomanip>
#include <filesystem>

#include "dbcppp/Network.h"
#include "dbcppp/Network2Functions.h"

#include "Config.h"

#include <boost/test/unit_test.hpp>
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(DBCParserTest)
{
    std::size_t i = 0;
    for (const auto& dbc_file : std::filesystem::directory_iterator(std::filesystem::path(TEST_FILES_PATH) / "dbc"))
    {
        if (dbc_file.path().extension() != ".dbc")
        {
            continue;
        }
        auto dbc_file_tmp = dbc_file.path().string() + ".tmp";
        std::unique_ptr<dbcppp::Network> spec;
        std::unique_ptr<dbcppp::Network> test;
        {
            std::ifstream dbc(dbc_file);
            spec = dbcppp::Network::loadDBCFromIs(dbc);
            std::ofstream tmp_dbc(dbc_file_tmp);
            bool open = tmp_dbc.is_open();
            using namespace dbcppp::Network2DBC;
            tmp_dbc << std::setprecision(32);
            tmp_dbc << *spec;
        }
        {
            std::ifstream dbc(dbc_file_tmp);
            test = dbcppp::Network::loadDBCFromIs(dbc);
        }
        auto error_msg = "Failed for " + std::to_string(i) + "th file ('" + dbc_file.path().string() + "')";
        BOOST_REQUIRE_MESSAGE(*spec == *test, error_msg);
        std::filesystem::remove(dbc_file_tmp);
        i++;
    }
}
