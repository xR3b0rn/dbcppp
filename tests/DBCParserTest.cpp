
#include <fstream>
#include <iomanip>
#include <filesystem>

#include "dbcppp/Network.h"
#include "dbcppp/Network2Functions.h"

#include "Config.h"

#include "Catch2.h"

TEST_CASE("DBCParserTest", "[]")
{
    std::size_t i = 0;
    for (const auto& dbc_file : std::filesystem::directory_iterator(std::filesystem::path(TEST_FILES_PATH) / "dbc"))
    {
        //BOOST_TEST_CHECKPOINT("DBCParserTest: Testing file '" + dbc_file.path().string() + "'");
        if (dbc_file.path().extension() != ".dbc")
        {
            continue;
        }
        std::cout << "Testing DBC grammar with file: " << dbc_file.path() << std::endl;
        auto dbc_file_tmp = dbc_file.path().string() + ".tmp";
        std::unique_ptr<dbcppp::INetwork> spec;
        std::unique_ptr<dbcppp::INetwork> test;
        {
            std::ifstream dbc(dbc_file.path());
            spec = dbcppp::INetwork::LoadDBCFromIs(dbc);
            std::ofstream tmp_dbc(dbc_file_tmp);
            bool open = tmp_dbc.is_open();
            using namespace dbcppp::Network2DBC;
            tmp_dbc << std::setprecision(10);
            tmp_dbc << *spec << std::endl;
        }
        {
            std::ifstream dbc(dbc_file_tmp);
            test = dbcppp::INetwork::LoadDBCFromIs(dbc);
            REQUIRE(test);
        }
        auto error_msg = "Failed for " + std::to_string(i) + "th file ('" + dbc_file.path().string() + "')";
        if (*spec != *test)
        {
            std::cout << error_msg << std::endl;
        }
        REQUIRE(*spec == *test);
        std::filesystem::remove(dbc_file_tmp);
        i++;
    }
}
