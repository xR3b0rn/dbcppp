
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
    
    DBCPPP_API const dbcppp_Attribute* dbcppp_AttributeCreate(
        const char* name,
        dbcppp_ObjectType object_type,
        dbcppp_AttributeValueType value_type,
        const void* value);
    DBCPPP_API void dbcppp_AttributeFree(const dbcppp_Attribute* attribute);
    DBCPPP_API const char* dbcppp_AttributeGetName(const dbcppp_Attribute* attribute);
    DBCPPP_API dbcppp_ObjectType dbcppp_AttributeGetObjectType(const dbcppp_Attribute* attribute);
    DBCPPP_API dbcppp_AttributeValueType dbcppp_AttributeGetValueType(const dbcppp_Attribute* attribute);
    DBCPPP_API int64_t dbcppp_AttributeGetValueAsInt(const dbcppp_Attribute* attribute);
    DBCPPP_API double dbcppp_AttributeGetValueAsDouble(const dbcppp_Attribute* attribute);
    DBCPPP_API const char* dbcppp_AttributeGetValueAsString(const dbcppp_Attribute* attribute);
    
    DBCPPP_API dbcppp_AttributeDefinition* dbcppp_AttributeDefinitionCreate(
          const char* name
        , dbcppp_ObjectType object_type
        , dbcppp_AttributeDefinitionValueType value_type
        , const void* value);
    DBCPPP_API void dbcppp_AttributeDefinitionFree(const dbcppp_AttributeDefinition* attribute_defintion);
    DBCPPP_API dbcppp_ObjectType dbcppp_AttributeDefinitionGetObjectType(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API const char* dbcppp_AttributeDefinitionGetName(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API dbcppp_AttributeDefinitionValueType dbcppp_AttributeDefinitionGetValueType(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API int64_t dbcppp_AttributeDefinitionGetValueTypeAsInt_Minimum(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API int64_t dbcppp_AttributeDefinitionGetValueTypeAsInt_Maximum(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API uint64_t dbcppp_AttributeDefinitionGetValueTypeAsHex_Minimum(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API uint64_t dbcppp_AttributeDefinitionGetValueTypeAsHex_Maximum(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API double dbcppp_AttributeDefinitionGetValueTypeAsFloat_Minimum(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API double dbcppp_AttributeDefinitionGetValueTypeAsFloat_Maximum(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API void dbcppp_AttributeDefinitionForEachValueTypeEnum(const dbcppp_AttributeDefinition* attribute_definition, void(*cb)(const char*, void*), void* data);
    DBCPPP_API void dbcppp_AttributeDefinitionFindValueTypeEnum(const dbcppp_AttributeDefinition* attribute_definition, bool(*pred)(const char*, void*), void* data);

    DBCPPP_API const dbcppp_BitTiming* dbcppp_BitTimingCreate(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2);
    DBCPPP_API void dbcppp_BitTimingFree(const dbcppp_BitTiming* bit_timing);
    DBCPPP_API uint64_t dbcppp_BitTimingGetBaudrate(const dbcppp_BitTiming* bit_timing);
    DBCPPP_API uint64_t dbcppp_BitTimingGetBTR1(const dbcppp_BitTiming* bit_timing);
    DBCPPP_API uint64_t dbcppp_BitTimingGetBTR2(const dbcppp_BitTiming* bit_timing);

    DBCPPP_API const dbcppp_EnvironmentVariable* dbcppp_EnvironmentVariableCreate(
          const char* name
        , dbcppp_EnvironmentVariableVarType var_type
        , double minimum
        , double maximum
        , const char* unit
        , double initial_value
        , uint64_t ev_id
        , dbcppp_EnvironmentVariableAccessType access_type
        , const char** access_nodes
        , dbcppp_ValueDescriptionPair** value_descriptions
        , uint64_t data_size
        , dbcppp_Attribute** attribute_values
        , const char* comment);
    DBCPPP_API void dbcppp_EnvironmentVariableFree(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API const char* dbcppp_EnvironmentVariableGetName(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API dbcppp_EnvironmentVariableVarType dbcppp_EnvironmentVariableGetVarType(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API double dbcppp_EnvironmentVariableGetMinimum(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API double dbcppp_EnvironmentVariableGetMaximum(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API const char* dbcppp_EnvironmentVariableGetUnit(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API double dbcppp_EnvironmentVariableGetInitialValue(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableGetEvId(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API dbcppp_EnvironmentVariableAccessType dbcppp_EnvironmentVariableGetAccessType(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableHasNode(const dbcppp_EnvironmentVariable* env_var, const char* node_name);
    DBCPPP_API void dbcppp_EnvironmentVariableForEachAccessNode(const dbcppp_EnvironmentVariable* env_var, void(*cb)(const char*, void*), void* data);
    DBCPPP_API void dbcppp_EnvironmentVariableForEachValueDescription(const dbcppp_EnvironmentVariable* env_var, void(*cb)(double, const char*, void*), void* data);
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableGetDataSize(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API dbcppp_Attribute* dbcppp_EnvironmentVariableGetAttributeValueByName(const dbcppp_EnvironmentVariable* env_var, const char* attribute_name);
    DBCPPP_API dbcppp_Attribute* dbcppp_EnvironmentVariableFindAttributeValue(const dbcppp_EnvironmentVariable* env_var, bool(*pred)(dbcppp_Attribute*, void*), void* data);
    DBCPPP_API void dbcppp_EnvironmentVariableForEachAttributeValue(const dbcppp_EnvironmentVariable* env_var, void(*cb)(dbcppp_Attribute*, void*), void* data);
    DBCPPP_API const char* dbcppp_EnvironmentVariableGetComment(const dbcppp_EnvironmentVariable* env_var);

    DBCPPP_API const dbcppp_Message* dbcppp_MessageCreate(
          uint64_t id
        , const char* name
        , uint64_t message_size
        , const char* transmitter
        , const char** message_transmitters
        , dbcppp_Signal** signals
        , dbcppp_Attribute** attribute_values
        , const char* comment);
    DBCPPP_API void dbcppp_MessageFree(const dbcppp_Message* msg);
    DBCPPP_API uint64_t dbcppp_MessageGetId(const dbcppp_Message* msg);
    DBCPPP_API const char* dbcppp_MessageGetName(const dbcppp_Message* msg);
    DBCPPP_API uint64_t dbcppp_MessageGetMessageSize(const dbcppp_Message* msg);
    DBCPPP_API const char* dbcppp_MessageGetTransmitter(const dbcppp_Message* msg);
    DBCPPP_API bool dbcppp_MessageHasMessageTransmitter(const dbcppp_Message* msg, const char* name);
    DBCPPP_API void dbcppp_MessageForEachMessageTransmitter(const dbcppp_Message* msg, void(*cb)(const char*, void*), void* data);
    DBCPPP_API const dbcppp_Signal* dbcppp_MessageGetSignalByName(const dbcppp_Message* msg, const char* name);
    DBCPPP_API const dbcppp_Signal* dbcppp_MessageFindSignal(const dbcppp_Message* msg, bool(*pred)(const dbcppp_Signal*, void*), void* data);
    DBCPPP_API void dbcppp_MessageForEachSignal(const dbcppp_Message* msg, void(*cb)(const dbcppp_Signal*, void*), void* data);
    DBCPPP_API const dbcppp_Attribute* dbcppp_MessageGetAttributeValueByName(const dbcppp_Message* msg, const char* name);
    DBCPPP_API const dbcppp_Attribute* dbcppp_MessageFindAttributeValue(dbcppp_Message* msg, bool(*pred)(const dbcppp_Attribute*, void*), void* data);
    DBCPPP_API void dbcppp_MessageForEachAttributeValue(dbcppp_Message* msg, void(*cb)(const dbcppp_Attribute*, void*), void* data);
    DBCPPP_API const char* dbcppp_MessageGetComment(const dbcppp_Message* msg);

    DBCPPP_API const dbcppp_Network* dbcppp_NetworkLoadDBCFromFile(const char* filename);
    DBCPPP_API const dbcppp_Network* dbcppp_NetworkCreate(
          const char* version
        , const char** new_symbols
        , dbcppp_BitTiming* bit_timing
        , dbcppp_Node** nodes
        , dbcppp_ValueTable** value_tables
        , dbcppp_Message** messages
        , dbcppp_EnvironmentVariable** environment_variables
        , dbcppp_AttributeDefinition** attribute_definitions
        , dbcppp_Attribute** attribute_defaults
        , dbcppp_Attribute** attribute_values
        , const char* comment);
    DBCPPP_API void dbcppp_NetworkFree(const dbcppp_Network* network);
    DBCPPP_API const char* dbcppp_NetworkGetVersion(const dbcppp_Network* net);
    DBCPPP_API bool dbcppp_NetworkHasNewSymbol(const dbcppp_Network* net, const char* new_symbol);
    DBCPPP_API void dbcppp_NetworkForEachNewSymbol(const dbcppp_Network* net, void(*cb)(const char*, void*), void* data);
    DBCPPP_API const dbcppp_BitTiming* dbcppp_NetworkGetBitTiming(const dbcppp_Network* net);
    DBCPPP_API const dbcppp_Node* dbcppp_NetworkGetNodeByName(const dbcppp_Network* net, const char* name);
    DBCPPP_API void dbcppp_NetworkForEachNode(const dbcppp_Network* net, void(*cb)(const dbcppp_Node*, void*), void* data);
    DBCPPP_API const dbcppp_ValueTable* dbcppp_NetworkGetValueTableByName(const dbcppp_Network* net, const char* name);
    DBCPPP_API const dbcppp_ValueTable* dbcppp_NetworkFindValueTable(const dbcppp_Network* net, bool(*pred)(const dbcppp_ValueTable*, void*), void* data);
    DBCPPP_API void dbcppp_NetworkForEachValueTable(const dbcppp_Network* net, void(*cb)(const dbcppp_ValueTable*, void*), void* data);
    DBCPPP_API const dbcppp_Message* dbcppp_NetworkGetMessageById(const dbcppp_Network* net, uint64_t id);
    DBCPPP_API const dbcppp_Message* dbcppp_NetworkFindMessage(const dbcppp_Network* net, bool(*pred)(const dbcppp_Message*, void*), void* data);
    DBCPPP_API void dbcppp_NetworkForEachMessage(const dbcppp_Network* net, void(*cb)(const dbcppp_Message*));
    DBCPPP_API const dbcppp_EnvironmentVariable* dbcppp_NetworkGetEnvironmentVariableByName(const dbcppp_Network* net, const char* name);
    DBCPPP_API dbcppp_EnvironmentVariable* dbcppp_NetworkFindEnvironmentVariable(const dbcppp_Network* net, bool(*pred)(const dbcppp_EnvironmentVariable*, void*), void* data);
    DBCPPP_API void dbcppp_NetworkForEachEnvironmentVariable(const dbcppp_Network* net, void(*cb)(const dbcppp_EnvironmentVariable*, void*), void* data);
    DBCPPP_API const dbcppp_AttributeDefinition* dbcppp_NetworkGetAttributeDefinitionByName(const dbcppp_Network* net, const char* name);
    DBCPPP_API dbcppp_AttributeDefinition* dbcppp_NetworkFindAttributeDefinition(const dbcppp_Network* net, bool(*pred)(const dbcppp_AttributeDefinition*, void*), void* data);
    DBCPPP_API void dbcppp_NetworkForEachAttributeDefinition(const dbcppp_Network* net, void(*cb)(const dbcppp_AttributeDefinition*, void*), void* data);
    DBCPPP_API const dbcppp_Attribute* dbcppp_NetworkGetAttributeDefaultByName(const dbcppp_Network* net, const char* name);
    DBCPPP_API const dbcppp_Attribute* dbcppp_NetworkFindAttributeDefault(const dbcppp_Network* net, bool(*pred)(const dbcppp_Attribute*, void*), void* data);
    DBCPPP_API void dbcppp_NetworkForEachAttributeDefault(const dbcppp_Network* net, void(*cb)(const dbcppp_Attribute*, void*), void* data);
    DBCPPP_API const dbcppp_Attribute* dbcppp_NetworkGetAttributeValueByName(const dbcppp_Network* net, const char* name);
    DBCPPP_API const dbcppp_Attribute* dbcppp_NetworkFindAttributeValue(const dbcppp_Network* net, bool(*pred)(const dbcppp_Attribute*, void*), void* data);
    DBCPPP_API void dbcppp_NetworkForEachAttributeValue(const dbcppp_Network* net, void(*cb)(const dbcppp_Attribute*, void*), void* data);
    DBCPPP_API const char* dbcppp_NetworkGetComment(const dbcppp_Network* net);

    DBCPPP_API const dbcppp_Node* dbcppp_NodeCreate(const char* name, const char* comment, dbcppp_Attribute** attributes);
    DBCPPP_API void dbcppp_NodeFree(const dbcppp_Node* node);
    DBCPPP_API const char* dbcppp_NodeGetName(const dbcppp_Node* node);
    DBCPPP_API const dbcppp_Attribute* dbcppp_NodeGetAttributeValueByName(const dbcppp_Node* node, const char* attribute_name);
    DBCPPP_API const dbcppp_Attribute* dbcppp_NodeFindAttributeValue(const dbcppp_Node* node, bool(*pred)(const dbcppp_Attribute*, void*), void* data);
    DBCPPP_API void dbcppp_NodeForEachAttributeValue(const dbcppp_Node* node, void(*cb)(const dbcppp_Attribute*, void*), void* data);
    DBCPPP_API const char* dbcppp_NodeGetComment(const dbcppp_Node* node);

    DBCPPP_API const dbcppp_Signal* dbcppp_SignalCreate(
          uint64_t message_size
        , const char* name
        , dbcppp_SignalMultiplexer multiplexer_indicator
        , uint64_t multiplexer_switch_value
        , uint64_t start_bit
        , uint64_t bit_size
        , dbcppp_SignalByteOrder byte_order
        , dbcppp_SignalValueType value_type
        , double factor
        , double offset
        , double minimum
        , double maximum
        , const char* unit
        , const char** receivers
        , dbcppp_Attribute** attribute_values
        , dbcppp_ValueDescriptionPair** value_descriptions
        , const char* comment
        , dbcppp_SignalExtendedValueType extended_value_type);
    DBCPPP_API const char* dbcppp_SignalGetName(const dbcppp_Signal* sig);
    DBCPPP_API dbcppp_SignalMultiplexer dbcppp_SignalGetMultiplexerIndicator(const dbcppp_Signal* sig);
    DBCPPP_API uint64_t dbcppp_SignalGetMultiplexerSwitchValue(const dbcppp_Signal* sig);
    DBCPPP_API uint64_t dbcppp_SignalGetStartBit(const dbcppp_Signal* sig);
    DBCPPP_API uint64_t dbcppp_SignalGetBitSize(const dbcppp_Signal* sig);
    DBCPPP_API dbcppp_SignalByteOrder dbcppp_SignalGetByteOrder(const dbcppp_Signal* sig);
    DBCPPP_API dbcppp_SignalValueType dbcppp_SignalGetValueType(const dbcppp_Signal* sig);
    DBCPPP_API double dbcppp_SignalGetFactor(const dbcppp_Signal* sig);
    DBCPPP_API double dbcppp_SignalGetOffset(const dbcppp_Signal* sig);
    DBCPPP_API double dbcppp_SignalGetMinimum(const dbcppp_Signal* sig);
    DBCPPP_API double dbcppp_SignalGetMaximum(const dbcppp_Signal* sig);
    DBCPPP_API const char* dbcppp_SignalGetUnit(const dbcppp_Signal* sig);
    DBCPPP_API bool dbcppp_SignalHasReceiver(const dbcppp_Signal* sig, const char* name);
    DBCPPP_API void dbcppp_SignalForEachReceiver(const dbcppp_Signal* sig, void(*cb)(const char*, void*), void* data);
    DBCPPP_API void dbcppp_SignalForEachValueDescription(const dbcppp_Signal* sig, void(*cb)(double, const char*, void*), void* data);
    DBCPPP_API const dbcppp_Attribute* dbcppp_SignalGetAttributeValueByName(const dbcppp_Signal* sig, const char* name);
    DBCPPP_API const dbcppp_Attribute* dbcppp_SignalFindAttributeValue(const dbcppp_Signal* sig, bool(*pred)(const dbcppp_Attribute*, void*), void* data);
    DBCPPP_API void dbcppp_SignalForEachAttributeValue(const dbcppp_Signal* sig, void(*cb)(const dbcppp_Attribute*, void*), void* data);
    DBCPPP_API const char* dbcppp_SignalGetComment(const dbcppp_Signal* sig);
    DBCPPP_API dbcppp_SignalExtendedValueType dbcppp_SignalGetGetExtendedValueType(const dbcppp_Signal* sig);
    DBCPPP_API double dbcppp_SignalDecode(const dbcppp_Signal* sig, const void* bytes);
    DBCPPP_API void dbcppp_SignalEncode(const dbcppp_Signal* sig, double raw, void* buffer);
    DBCPPP_API double dbcppp_SignalRawToPhys(const dbcppp_Signal* sig, double raw);
    DBCPPP_API double dbcppp_SignalPhysToRaw(const dbcppp_Signal* sig, double phys);

    DBCPPP_API const dbcppp_SignalType* dbcppp_SignalTypeCreate(
          const char* name
        , uint64_t signal_size
        , dbcppp_SignalByteOrder byte_order
        , dbcppp_SignalValueType value_type
        , double factor
        , double offset
        , double minimum
        , double maximum
        , const char* unit
        , double default_value
        , const char* value_table);
    DBCPPP_API const char* dbcppp_SignalTypeGetName(const dbcppp_SignalType* sig_type);
    DBCPPP_API uint64_t dbcppp_SignalTypeGetSignalSize(const dbcppp_SignalType* sig_type);
    DBCPPP_API dbcppp_SignalByteOrder dbcppp_SignalTypeGetByteOrder(const dbcppp_SignalType* sig_type);
    DBCPPP_API dbcppp_SignalValueType dbcppp_SignalTypeGetValueType(const dbcppp_SignalType* sig_type);
    DBCPPP_API double dbcppp_SignalTypeGetFactor(const dbcppp_SignalType* sig_type);
    DBCPPP_API double dbcppp_SignalTypeGetOffset(const dbcppp_SignalType* sig_type);
    DBCPPP_API double dbcppp_SignalTypeGetMinimum(const dbcppp_SignalType* sig_type);
    DBCPPP_API double dbcppp_SignalTypeGetMaximum(const dbcppp_SignalType* sig_type);
    DBCPPP_API const char* dbcppp_SignalTypeGetUnit(const dbcppp_SignalType* sig_type);
    DBCPPP_API double dbcppp_SignalTypeGetDefaultValue(const dbcppp_SignalType* sig_type);
    DBCPPP_API const char* dbcppp_SignalTypeGetValueTable(const dbcppp_SignalType* sig_type);
    DBCPPP_API const dbcppp_ValueTable* dbcppp_ValueTableCreate(const char* name, dbcppp_SignalType* signal_type, dbcppp_ValueDescriptionPair** pairs);
    DBCPPP_API void dbcppp_ValueTableFree(const dbcppp_ValueTable* value_table);
    DBCPPP_API const char* dbcppp_ValueTableGetName(const dbcppp_ValueTable* value_table);
    DBCPPP_API const dbcppp_SignalType* dbcppp_ValueTableGetSignalType(const dbcppp_ValueTable* value_table);
    DBCPPP_API void dbcppp_ValueTableForEachValueEncodingDescription(const dbcppp_ValueTable* value_table, void(*cb)(double, const char*, void*), void* data);

#ifdef __cplusplus
}
#endif
