
#include <fstream>

#include "libdbcppp/DBCParser.h"

//#include "libdbcppp/DBCGrammarPEGLT.h"
#include "Config.h"

#include <boost/test/unit_test.hpp>
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(PEGTL_grammar_test)
{
    const char* str_double = "1e10abcd";
    double r_double;
    auto iter = DBCIterator(str_double);
    DBCParser::ParseDouble(iter, r_double);

    const char* str_int = "0x1e10abcd";
    int64_t r_int;
    iter = DBCIterator(str_int);
    DBCParser::ParseInt(iter, r_int);

    const char* str_new_sym = "BA_DEF_SGTYPE_ asdf";
    iter = DBCIterator(str_new_sym);
    DBCParser::ParseNewSymbol(iter);

    const char* str_new_syms =
        "NS_: SG_MUL_VAL_ // A Comment\n"
        " /*another comment*/   ENVVAR_DATA_";
    iter = DBCIterator(str_new_syms);
    DBCParser::ParseNewSymbols(iter);

    const char* str_nodes =
        "BU_: Bus0 Bus1 Bus2 Receiver";
    std::vector<dbcppp::G_Node> nodes;
    iter = DBCIterator(str_nodes);
    DBCParser::ParseNodes(iter, nodes);
    
    const char* str_messages =
        "BO_ 1 test_msg_mux0: 8 VehicleBus\n"
        " SG_ mux_switch_value M : 0|3@1+ (1,0) [0|7] \"\" Receiver\n"
        " SG_ s0_0 m0 : 3|10@1+ (10,0) [0|10230] \"unit\" Receiver\n"
        " SG_ s0_1 m0 : 13|10@1+ (10,0) [0|4294967295] \"unit\" Receiver\n"
        " SG_ s0_2 m0 : 23|10@1+ (10,0) [0|0] \"unit\" Receiver\n"
        " SG_ s0_3 m0 : 33|10@1+ (10,0) [0|0] \"unit\" Receiver\n"
        " SG_ s0_4 m0 : 43|7@1+ (2,0) [0|0] \"V\" Receiver\n"
        " SG_ s0_5 m0 : 50|7@1+ (2,0) [0|0] \"V\" Receiver\n"
        " SG_ s0_6 m0 : 57|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s0_7 m0 : 58|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s0_8 m0 : 59|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s0_9 m0 : 60|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s0_10 m0 : 61|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s0_11 m0 : 62|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s0_12 m0 : 63|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s1_0 m1 : 8|7@1+ (2,0) [0|0] \"V\" Receiver\n"
        " SG_ s1_1 m1 : 16|10@1+ (10,0) [0|0] \"rpm\" Receiver\n"
        " SG_ s1_2 m1 : 26|10@1+ (10,0) [0|0] \"rpm\" Receiver\n"
        " SG_ s1_3 m1 : 36|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s1_4 m1 : 37|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s1_5 m1 : 38|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s1_6 m1 : 40|8@1+ (0.5,-25) [0|0] \"A\" Receiver\n"
        " SG_ s2_0 m2 : 3|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s2_1 m2 : 4|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s2_2 m2 : 5|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s2_3 m2 : 6|10@1+ (0.25,-70) [0|0] \"deg\" Receiver\n"
        " SG_ s2_4 m2 : 16|10@1+ (0.25,-10) [0|0] \"deg\" Receiver\n"
        " SG_ s2_5 m2 : 26|7@1+ (1,0) [0|0] \"%\" Receiver\n"
        " SG_ s2_6 m2 : 33|2@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s2_7 m2 : 35|8@1- (1,0) [0|0] \"%\" Receiver\n"
        " SG_ s2_8 m2 : 43|10@1+ (5,0) [0|0] \"deg\" Receiver\n"
        " SG_ s2_9 m2 : 53|6@1+ (1,0) [0|0] \"A\" Receiver\n"
        " SG_ s3_0 m3 : 8|8@1+ (1,-40) [0|0] \"C\" Receiver\n"
        " SG_ s3_1 m3 : 16|11@1+ (0.5,1) [0|0] \"C\" Receiver\n"
        " SG_ s3_2 m3 : 27|10@1+ (1,-20) [0|0] \"C\" Receiver\n"
        " SG_ s3_3 m3 : 37|10@1+ (1,0) [0|0] \"C\" Receiver\n"
        " SG_ s3_4 m3 : 47|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s3_5 m3 : 48|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s3_6 m3 : 49|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s3_7 m3 : 50|11@1+ (0.125,-40) [0|0] \"C\" Receiver\n"
        " SG_ s3_8 m3 : 61|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s3_9 m3 : 62|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s3_10 m3 : 63|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_0 m4 : 3|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_1 m4 : 8|7@1+ (1,0) [0|0] \"%\" Receiver\n"
        " SG_ s4_2 m4 : 16|7@1+ (1,0) [0|0] \"%\" Receiver\n"
        " SG_ s4_3 m4 : 24|2@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_4 m4 : 26|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_5 m4 : 27|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_6 m4 : 28|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_7 m4 : 29|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_8 m4 : 30|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_9 m4 : 31|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_10 m4 : 32|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_11 m4 : 33|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_12 m4 : 34|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_13 m4 : 35|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_14 m4 : 36|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_15 m4 : 37|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_16 m4 : 38|1@1+ (1,0) [0|0] \"\" Receiver\n"
        " SG_ s4_17 m4 : 40|6@1+ (1,0) [0|0] \"%/s\" Receiver\n"
        " SG_ s4_18 m4 : 46|1@1+ (1,0) [0|0] \"\" Receiver\n";
    std::vector<dbcppp::G_Message> messages;
    iter = DBCIterator(str_messages);
    DBCParser::ParseMessages(iter, messages);

    //// Set up the states, here a single std::string as that is
    //// what our action requires as additional function argument.
    //std::string version;
    //std::string dbc_file(TEST_DBC);

    ////BOOST_TEST_MESSAGE("Testing PEGTL grammar...");

    //file_input in("C:/Users/hinde/OneDrive/Desktop/test2.dbc");
    ////memory_input in("NS_:\r\nNS_DESC_", "");
    //// Start the parsing run with our grammar, action and state.
    //G_Network net;
    //try
    //{
    //  auto result = tao::pegtl::parse<_network, _action>(in, net);
    //  //auto result = tao::pegtl::parse_tree::parse<_network, _action>(in, version);
    //  //if (result->children[0]->is_type<_network>())
    //  //{
    //  //  auto str = result->children[0]->children[0]->string();
    //  //}
    //}
    //catch (...)
    //{
    //  std::cout << "err\n";
    //}
    return;
}
