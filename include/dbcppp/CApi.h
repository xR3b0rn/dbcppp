
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include "Export.h"
#include <stdint.h>

    typedef enum
    {
        dbcppp_ObjectType_Network,
        dbcppp_ObjectType_Node,
        dbcppp_ObjectType_Message,
        dbcppp_ObjectType_Signal,
        dbcppp_ObjectType_EnvironmentVariable
    } dbcppp_ObjectType;
    typedef enum
    {
        dbcppp_AttributeValueType_Int,
        dbcppp_AttributeValueType_Double,
        dbcppp_AttributeValueType_String
    } dbcppp_AttributeValueType;
    typedef enum
    {
        dbcppp_AttributeDefinitionValueTypeInt,
        dbcppp_AttributeDefinitionValueTypeHex,
        dbcppp_AttributeDefinitionValueTypeFloat,
        dbcppp_AttributeDefinitionValueTypeString,
        dbcppp_AttributeDefinitionValueTypeEnum
    } dbcppp_AttributeDefinitionValueType;
    typedef struct
    {
        double value;
        const char* description;
    } dbcppp_ValueDescriptionPair;
    typedef enum
    {
        dbcppp_EnvironmentVariableVarTypeInteger,
        dbcppp_EnvironmentVariableVarTypeFloat,
        dbcppp_EnvironmentVariableVarTypeString,
        dbcppp_EnvironmentVariableVarTypeData
    } dbcppp_EnvironmentVariableVarType;
    typedef enum
    {
        dbcppp_EnvironmentVariableAccessTypeUnrestricted,
        dbcppp_EnvironmentVariableAccessTypeRead,
        dbcppp_EnvironmentVariableAccessTypeWrite,
        dbcppp_EnvironmentVariableAccessTypeReadWrite
    } dbcppp_EnvironmentVariableAccessType;
    typedef enum
    {
        dbcppp_SignalMultiplexerNoMux,
        dbcppp_SignalMultiplexerMuxSwitch,
        dbcppp_SignalMultiplexerMuxValue
    } dbcppp_SignalMultiplexer;
    typedef enum
    {
        dbcppp_SignalByteOrderLittleEndian,
        dbcppp_SignalByteOrderBigEndian
    } dbcppp_SignalByteOrder;
    typedef enum
    {
        dbcppp_SignalValueTypeSigned,
        dbcppp_SignalValueTypeUnsigned
    } dbcppp_SignalValueType;
    typedef enum
    {
        dbcppp_SignalExtendedValueTypeInteger,
        dbcppp_SignalExtendedValueTypeFloat, 
        dbcppp_SignalExtendedValueTypeDouble
    } dbcppp_SignalExtendedValueType;

    typedef struct {} dbcppp_Network;
    typedef struct {} dbcppp_BitTiming;
    typedef struct {} dbcppp_Node;
    typedef struct {} dbcppp_ValueTable;
    typedef struct {} dbcppp_Message;
    typedef struct {} dbcppp_EnvironmentVariable;
    typedef struct {} dbcppp_AttributeDefinition;
    typedef struct {} dbcppp_Attribute;
    typedef struct {} dbcppp_SignalType;
    typedef struct {} dbcppp_Signal;
    
#ifdef __cplusplus
}
#endif
