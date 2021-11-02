
#include "Catch2.h"
#include <dbcppp/CApi.h>
#include <dbcppp/Network.h>

using namespace dbcppp;

TEST_CASE("API Test: AttributeDefinition", "[]")
{
    constexpr char* test_dbc =
        "VERSION \"\"\n"
        "NS_ :\n"
        "BS_:\n"
        "BU_:\n"
        "BA_DEF_ BO_  \"AD_Name\" INT 1 3000;";
    
    SECTION("CPP API")
    {
        std::istringstream iss(test_dbc);
        auto net = INetwork::LoadDBCFromIs(iss);
        REQUIRE(net);

        REQUIRE(net->AttributeDefinitions_Size() == 1);
        REQUIRE(net->AttributeDefinitions_Get(0).ObjectType() == IAttributeDefinition::EObjectType::Message);
        REQUIRE(net->AttributeDefinitions_Get(0).Name() == "AD_Name");
        auto value_type = std::get_if<IAttributeDefinition::ValueTypeInt>(&net->AttributeDefinitions_Get(0).ValueType());
        REQUIRE(value_type);
        REQUIRE(value_type->minimum == 1);
        REQUIRE(value_type->maximum == 3000);
    }
    SECTION("C API")
    {
        auto net = dbcppp_NetworkLoadDBCFromMemory(test_dbc);
        REQUIRE(net);

        REQUIRE(dbcppp_NetworkAttributeDefinitions_Size(net) == 1);
        auto attr_def = dbcppp_NetworkAttributeDefinitions_Get(net, 0);
        REQUIRE(dbcppp_AttributeDefinitionObjectType(attr_def) == dbcppp_ObjectType_Message);
        REQUIRE(dbcppp_AttributeDefinitionName(attr_def) == std::string("AD_Name"));
        REQUIRE(dbcppp_AttributeDefinitionValueType(attr_def) == dbcppp_AttributeDefinitionValueTypeInt);
        REQUIRE(dbcppp_AttributeDefinitionValueTypeAsInt_Minimum(attr_def) == 1);
        REQUIRE(dbcppp_AttributeDefinitionValueTypeAsInt_Maximum(attr_def) == 3000);
    }
}
TEST_CASE("API Test: BitTiming", "[]")
{
    constexpr char* test_dbc =
        "VERSION \"\"\n"
        "NS_ :\n"
        "BS_: 1 : 2, 3\n"
        "BU_:\n";
    
    SECTION("CPP API")
    {
        std::istringstream iss(test_dbc);
        auto net = INetwork::LoadDBCFromIs(iss);
        REQUIRE(net);

        REQUIRE(net->BitTiming().Baudrate() == 1);
        REQUIRE(net->BitTiming().BTR1() == 2);
        REQUIRE(net->BitTiming().BTR2() == 3);
    }
    SECTION("C API")
    {
        auto net = dbcppp_NetworkLoadDBCFromMemory(test_dbc);
        REQUIRE(net);

        auto bt = dbcppp_NetworkBitTiming(net);
        REQUIRE(bt);
        REQUIRE(dbcppp_BitTimingBaudrate(bt) == 1);
        REQUIRE(dbcppp_BitTimingBTR1(bt) == 2);
        REQUIRE(dbcppp_BitTimingBTR2(bt) == 3);
    }
}
TEST_CASE("API Test: EnvironmentVariable", "[]")
{
    constexpr char* test_dbc =
        "VERSION \"\"\n"
        "NS_ :\n"
        "BS_:\n"
        "BU_:\n"
        "EV_ EnvVarName : 0 [1|2] \"Unit\" 5.5 1 DUMMY_NODE_VECTOR0 Node0;\n"
        "CM_ EV_ EnvVarName \"Comment\";\n";
    
    SECTION("CPP API")
    {
        std::istringstream iss(test_dbc);
        auto net = INetwork::LoadDBCFromIs(iss);
        REQUIRE(net);

        REQUIRE(net->EnvironmentVariables_Size() == 1);
        REQUIRE(net->EnvironmentVariables_Get(0).Name() == "EnvVarName");
        REQUIRE(net->EnvironmentVariables_Get(0).VarType() == IEnvironmentVariable::EVarType::Integer);
        REQUIRE(net->EnvironmentVariables_Get(0).Minimum() == 1);
        REQUIRE(net->EnvironmentVariables_Get(0).Maximum() == 2);
        REQUIRE(net->EnvironmentVariables_Get(0).Unit() == "Unit");
        REQUIRE(net->EnvironmentVariables_Get(0).InitialValue() == 5.5);
        REQUIRE(net->EnvironmentVariables_Get(0).EvId() == 1);
        REQUIRE(net->EnvironmentVariables_Get(0).AccessType() == IEnvironmentVariable::EAccessType::Unrestricted);
        REQUIRE(net->EnvironmentVariables_Get(0).AccessNodes_Size() == 1);
        REQUIRE(net->EnvironmentVariables_Get(0).AccessNodes_Get(0) == "Node0");
    }
    SECTION("C API")
    {
        auto net = dbcppp_NetworkLoadDBCFromMemory(test_dbc);
        REQUIRE(net);

        REQUIRE(dbcppp_NetworkEnvironmentVariables_Size(net) == 1);
        auto ev = dbcppp_NetworkEnvironmentVariables_Get(net, 0);
        REQUIRE(ev);
        REQUIRE(dbcppp_EnvironmentVariableName(ev) == std::string("EnvVarName"));
        REQUIRE(dbcppp_EnvironmentVariableVarType(ev) == dbcppp_EnvironmentVariableVarTypeInteger);
        REQUIRE(dbcppp_EnvironmentVariableMinimum(ev) == 1);
        REQUIRE(dbcppp_EnvironmentVariableMaximum(ev) == 2);
        REQUIRE(dbcppp_EnvironmentVariableUnit(ev) == std::string("Unit"));
        REQUIRE(dbcppp_EnvironmentVariableInitialValue(ev) == 5.5);
        REQUIRE(dbcppp_EnvironmentVariableEvId(ev) == 1);
        REQUIRE(dbcppp_EnvironmentVariableAccessType(ev) == dbcppp_EnvironmentVariableAccessTypeUnrestricted);
        REQUIRE(dbcppp_EnvironmentVariableAccessNodes_Size(ev) == 1);
        REQUIRE(dbcppp_EnvironmentVariableAccessNodes_Get(ev, 0) == std::string("Node0"));
    }
}
TEST_CASE("API Test: Signal", "[]")
{
    constexpr char* test_dbc =
        "VERSION \"\"\n"
        "NS_ :\n"
        "BS_: 1 : 2, 3\n"
        "BU_:\n"
        "BO_ 1 Msg0: 8 Sender0\n"
        "  SG_ Sig0: 0|1@1+ (1,0) [1|12] \"Unit0\" Vector__XXX\n"
        "  SG_ Sig1 m0 : 1|1@0- (1,0) [1|12] \"Unit1\" Recv0, Recv1\n"
        "  SG_ Sig2 M : 2|1@0- (1,0) [1|12] \"Unit2\" Recv0, Recv1\n";
    
    SECTION("CPP API")
    {
        std::istringstream iss(test_dbc);
        auto net = INetwork::LoadDBCFromIs(iss);
        REQUIRE(net);

        REQUIRE(net->Messages_Size() == 1);
        REQUIRE(net->Messages_Get(0).Signals_Size() == 3);
        REQUIRE(net->Messages_Get(0).Signals_Get(0).Name() == "Sig0");
        REQUIRE(net->Messages_Get(0).Signals_Get(0).MultiplexerIndicator() == ISignal::EMultiplexer::NoMux);
        REQUIRE(net->Messages_Get(0).Signals_Get(0).StartBit() == 0);
        REQUIRE(net->Messages_Get(0).Signals_Get(0).BitSize() == 1);
        REQUIRE(net->Messages_Get(0).Signals_Get(0).ByteOrder() == ISignal::EByteOrder::LittleEndian);
        REQUIRE(net->Messages_Get(0).Signals_Get(0).ValueType() == ISignal::EValueType::Unsigned);
        REQUIRE(net->Messages_Get(0).Signals_Get(0).Factor() == 1);
        REQUIRE(net->Messages_Get(0).Signals_Get(0).Offset() == 0);
        REQUIRE(net->Messages_Get(0).Signals_Get(0).Minimum() == 1);
        REQUIRE(net->Messages_Get(0).Signals_Get(0).Maximum() == 12);
        REQUIRE(net->Messages_Get(0).Signals_Get(0).Unit() == "Unit0");
        REQUIRE(net->Messages_Get(0).Signals_Get(0).Receivers_Size() == 1);
        REQUIRE(net->Messages_Get(0).Signals_Get(0).Receivers_Get(0) == "Vector__XXX");

        REQUIRE(net->Messages_Get(0).Signals_Get(1).Name() == "Sig1");
        REQUIRE(net->Messages_Get(0).Signals_Get(1).MultiplexerIndicator() == ISignal::EMultiplexer::MuxValue);
        REQUIRE(net->Messages_Get(0).Signals_Get(1).StartBit() == 1);
        REQUIRE(net->Messages_Get(0).Signals_Get(1).BitSize() == 1);
        REQUIRE(net->Messages_Get(0).Signals_Get(1).ByteOrder() == ISignal::EByteOrder::BigEndian);
        REQUIRE(net->Messages_Get(0).Signals_Get(1).ValueType() == ISignal::EValueType::Signed);
        REQUIRE(net->Messages_Get(0).Signals_Get(1).Factor() == 1);
        REQUIRE(net->Messages_Get(0).Signals_Get(1).Offset() == 0);
        REQUIRE(net->Messages_Get(0).Signals_Get(1).Minimum() == 1);
        REQUIRE(net->Messages_Get(0).Signals_Get(1).Maximum() == 12);
        REQUIRE(net->Messages_Get(0).Signals_Get(1).Unit() == "Unit1");
        REQUIRE(net->Messages_Get(0).Signals_Get(1).Receivers_Size() == 2);
        REQUIRE(net->Messages_Get(0).Signals_Get(1).Receivers_Get(0) == "Recv0");
        REQUIRE(net->Messages_Get(0).Signals_Get(1).Receivers_Get(1) == "Recv1");
        
        REQUIRE(net->Messages_Get(0).Signals_Get(2).Name() == "Sig2");
        REQUIRE(net->Messages_Get(0).Signals_Get(2).MultiplexerIndicator() == ISignal::EMultiplexer::MuxSwitch);
        REQUIRE(net->Messages_Get(0).Signals_Get(2).StartBit() == 2);
        REQUIRE(net->Messages_Get(0).Signals_Get(2).BitSize() == 1);
        REQUIRE(net->Messages_Get(0).Signals_Get(2).ByteOrder() == ISignal::EByteOrder::BigEndian);
        REQUIRE(net->Messages_Get(0).Signals_Get(2).ValueType() == ISignal::EValueType::Signed);
        REQUIRE(net->Messages_Get(0).Signals_Get(2).Factor() == 1);
        REQUIRE(net->Messages_Get(0).Signals_Get(2).Offset() == 0);
        REQUIRE(net->Messages_Get(0).Signals_Get(2).Minimum() == 1);
        REQUIRE(net->Messages_Get(0).Signals_Get(2).Maximum() == 12);
        REQUIRE(net->Messages_Get(0).Signals_Get(2).Unit() == "Unit2");
        REQUIRE(net->Messages_Get(0).Signals_Get(2).Receivers_Size() == 2);
        REQUIRE(net->Messages_Get(0).Signals_Get(2).Receivers_Get(0) == "Recv0");
        REQUIRE(net->Messages_Get(0).Signals_Get(2).Receivers_Get(1) == "Recv1");
    }
    SECTION("C API")
    {
        auto net = dbcppp_NetworkLoadDBCFromMemory(test_dbc);
        REQUIRE(net);

        REQUIRE(dbcppp_NetworkMessages_Size(net) == 1);
        auto msg = dbcppp_NetworkMessages_Get(net, 0);
        REQUIRE(msg);
        REQUIRE(dbcppp_MessageSignals_Size(msg) == 3);
        auto sig = dbcppp_MessageSignals_Get(msg, 0);
        REQUIRE(sig);
        REQUIRE(dbcppp_SignalName(sig) == std::string("Sig0"));
        REQUIRE(dbcppp_SignalMultiplexerIndicator(sig) == dbcppp_SignalMultiplexerNoMux);
        REQUIRE(dbcppp_SignalStartBit(sig) == 0);
        REQUIRE(dbcppp_SignalBitSize(sig) == 1);
        REQUIRE(dbcppp_SignalByteOrder(sig) == dbcppp_SignalByteOrderLittleEndian);
        REQUIRE(dbcppp_SignalValueType(sig) == dbcppp_SignalValueTypeUnsigned);
        REQUIRE(dbcppp_SignalFactor(sig) == 1);
        REQUIRE(dbcppp_SignalOffset(sig) == 0);
        REQUIRE(dbcppp_SignalMinimum(sig) == 1);
        REQUIRE(dbcppp_SignalMaximum(sig) == 12);
        REQUIRE(dbcppp_SignalReceivers_Size(sig) == 1);
        REQUIRE(dbcppp_SignalReceivers_Get(sig, 0) == std::string("Vector__XXX"));
    }
}
TEST_CASE("API Test: Message", "[]")
{
    constexpr char* test_dbc =
        "VERSION \"\"\n"
        "NS_ :\n"
        "BS_: 1 : 2, 3\n"
        "BU_:\n"
        "BO_ 1 Msg0: 8 Sender0\n"
        "  SG_ Sig0: 0|1@1+ (1,0) [1|12] \"Unit0\" Vector__XXX\n"
        "  SG_ Sig1 m0 : 1|1@0- (1,0) [1|12] \"Unit1\" Recv0, Recv1\n"
        "  SG_ Sig2 M : 2|1@0- (1,0) [1|12] \"Unit2\" Recv0, Recv1\n";
    
    SECTION("CPP API")
    {
        std::istringstream iss(test_dbc);
        auto net = INetwork::LoadDBCFromIs(iss);
        REQUIRE(net);

        REQUIRE(net->Messages_Size() == 1);
        REQUIRE(net->Messages_Get(0).Id() == 1);
        REQUIRE(net->Messages_Get(0).MessageSize() == 8);
        REQUIRE(net->Messages_Get(0).Transmitter() == "Sender0");
        REQUIRE(net->Messages_Get(0).Signals_Size() == 3);
    }
    SECTION("C API")
    {
        auto net = dbcppp_NetworkLoadDBCFromMemory(test_dbc);
        REQUIRE(net);

        REQUIRE(dbcppp_NetworkMessages_Size(net) == 1);
        auto msg = dbcppp_NetworkMessages_Get(net, 0);
        REQUIRE(msg);
        REQUIRE(dbcppp_MessageSignals_Size(msg) == 3);
    }
}
