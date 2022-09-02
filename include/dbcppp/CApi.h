
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
    } dbcppp_EObjectType;
    typedef enum
    {
        dbcppp_AttributeValueType_Int,
        dbcppp_AttributeValueType_Double,
        dbcppp_AttributeValueType_String
    } dbcppp_EAttributeValueType;
    typedef enum
    {
        dbcppp_AttributeDefinitionValueTypeInt,
        dbcppp_AttributeDefinitionValueTypeHex,
        dbcppp_AttributeDefinitionValueTypeFloat,
        dbcppp_AttributeDefinitionValueTypeString,
        dbcppp_AttributeDefinitionValueTypeEnum
    } dbcppp_EAttributeDefinitionValueType;
    typedef enum
    {
        dbcppp_EnvironmentVariableVarTypeInteger,
        dbcppp_EnvironmentVariableVarTypeFloat,
        dbcppp_EnvironmentVariableVarTypeString,
        dbcppp_EnvironmentVariableVarTypeData
    } dbcppp_EEnvironmentVariableVarType;
    typedef enum
    {
        dbcppp_EnvironmentVariableAccessTypeUnrestricted,
        dbcppp_EnvironmentVariableAccessTypeRead,
        dbcppp_EnvironmentVariableAccessTypeWrite,
        dbcppp_EnvironmentVariableAccessTypeReadWrite
    } dbcppp_EEnvironmentVariableAccessType;
    typedef enum
    {
        dbcppp_SignalMultiplexerNoMux,
        dbcppp_SignalMultiplexerMuxSwitch,
        dbcppp_SignalMultiplexerMuxValue
    } dbcppp_ESignalMultiplexer;
    typedef enum
    {
        dbcppp_SignalByteOrderLittleEndian,
        dbcppp_SignalByteOrderBigEndian
    } dbcppp_ESignalByteOrder;
    typedef enum
    {
        dbcppp_SignalValueTypeSigned,
        dbcppp_SignalValueTypeUnsigned
    } dbcppp_ESignalValueType;
    typedef enum
    {
        dbcppp_SignalExtendedValueTypeInteger,
        dbcppp_SignalExtendedValueTypeFloat, 
        dbcppp_SignalExtendedValueTypeDouble
    } dbcppp_ESignalExtendedValueType;

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
    typedef struct {} dbcppp_SignalMultiplexerValue;
    typedef struct {
        uint64_t from;
        uint64_t to;
    } dbcppp_ValueRange;
    typedef struct {} dbcppp_ValueEncodingDescription;
    
    DBCPPP_API const dbcppp_Attribute* dbcppp_AttributeCreate(
        const char* name,
        dbcppp_EObjectType object_type,
        dbcppp_EAttributeValueType value_type,
        const void* value);
    DBCPPP_API void dbcppp_AttributeFree(const dbcppp_Attribute* attribute);
    DBCPPP_API const char* dbcppp_AttributeName(const dbcppp_Attribute* attribute);
    DBCPPP_API dbcppp_EObjectType dbcppp_AttributeObjectType(const dbcppp_Attribute* attribute);
    DBCPPP_API dbcppp_EAttributeValueType dbcppp_AttributeValueType(const dbcppp_Attribute* attribute);
    DBCPPP_API int64_t dbcppp_AttributeValueAsInt(const dbcppp_Attribute* attribute);
    DBCPPP_API double dbcppp_AttributeValueAsDouble(const dbcppp_Attribute* attribute);
    DBCPPP_API const char* dbcppp_AttributeValueAsString(const dbcppp_Attribute* attribute);
    
    DBCPPP_API dbcppp_AttributeDefinition* dbcppp_AttributeDefinitionCreate(
          const char* name
        , dbcppp_EObjectType object_type
        , dbcppp_EAttributeDefinitionValueType value_type
        , const void* value);
    DBCPPP_API void dbcppp_AttributeDefinitionFree(const dbcppp_AttributeDefinition* attribute_defintion);
    DBCPPP_API dbcppp_EObjectType dbcppp_AttributeDefinitionObjectType(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API const char* dbcppp_AttributeDefinitionName(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API dbcppp_EAttributeDefinitionValueType dbcppp_AttributeDefinitionValueType(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API int64_t dbcppp_AttributeDefinitionValueTypeAsInt_Minimum(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API int64_t dbcppp_AttributeDefinitionValueTypeAsInt_Maximum(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API uint64_t dbcppp_AttributeDefinitionValueTypeAsHex_Minimum(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API uint64_t dbcppp_AttributeDefinitionValueTypeAsHex_Maximum(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API double dbcppp_AttributeDefinitionValueTypeAsFloat_Minimum(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API double dbcppp_AttributeDefinitionValueTypeAsFloat_Maximum(const dbcppp_AttributeDefinition* attribute_definition);
    DBCPPP_API const char* dbcppp_AttributeDefinitionValueTypeAsEnum_Get(const dbcppp_AttributeDefinition* attribute_definition, uint64_t i);
    DBCPPP_API uint64_t dbcppp_AttributeDefinitionValueTypeAsEnum_Size(const dbcppp_AttributeDefinition* attribute_definition);

    DBCPPP_API const dbcppp_BitTiming* dbcppp_BitTimingCreate(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2);
    DBCPPP_API void dbcppp_BitTimingFree(const dbcppp_BitTiming* bit_timing);
    DBCPPP_API uint64_t dbcppp_BitTimingBaudrate(const dbcppp_BitTiming* bit_timing);
    DBCPPP_API uint64_t dbcppp_BitTimingBTR1(const dbcppp_BitTiming* bit_timing);
    DBCPPP_API uint64_t dbcppp_BitTimingBTR2(const dbcppp_BitTiming* bit_timing);

    DBCPPP_API const dbcppp_EnvironmentVariable* dbcppp_EnvironmentVariableCreate(
          const char* name
        , dbcppp_EEnvironmentVariableVarType var_type
        , double minimum
        , double maximum
        , const char* unit
        , double initial_value
        , uint64_t ev_id
        , dbcppp_EEnvironmentVariableAccessType access_type
        , const char** access_nodes
        , dbcppp_ValueEncodingDescription** value_encoding_descriptions
        , uint64_t data_size
        , dbcppp_Attribute** attribute_values
        , const char* comment);
    DBCPPP_API void dbcppp_EnvironmentVariableFree(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API const char* dbcppp_EnvironmentVariableName(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API dbcppp_EEnvironmentVariableVarType dbcppp_EnvironmentVariableVarType(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API double dbcppp_EnvironmentVariableMinimum(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API double dbcppp_EnvironmentVariableMaximum(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API const char* dbcppp_EnvironmentVariableUnit(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API double dbcppp_EnvironmentVariableInitialValue(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableEvId(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API dbcppp_EEnvironmentVariableAccessType dbcppp_EnvironmentVariableAccessType(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API const char* dbcppp_EnvironmentVariableAccessNodes_Get(const dbcppp_EnvironmentVariable* env_var, uint64_t i);
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableAccessNodes_Size(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableValueEncodingDescriptions_GetValue(const dbcppp_EnvironmentVariable* env_var, int64_t i);
    DBCPPP_API const char* dbcppp_EnvironmentVariableValueEncodingDescriptions_GetDescription(const dbcppp_EnvironmentVariable* env_var, int64_t i);
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableValueEncodingDescriptions_Size(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableDataSize(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API const dbcppp_Attribute* dbcppp_EnvironmentVariableAttributeValues_Get(const dbcppp_EnvironmentVariable* env_var, uint64_t i);
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableAttributeValues_Size(const dbcppp_EnvironmentVariable* env_var);
    DBCPPP_API const char* dbcppp_EnvironmentVariableComment(const dbcppp_EnvironmentVariable* env_var);

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
    DBCPPP_API uint64_t dbcppp_MessageId(const dbcppp_Message* msg);
    DBCPPP_API const char* dbcppp_MessageName(const dbcppp_Message* msg);
    DBCPPP_API uint64_t dbcppp_MessageMessageSize(const dbcppp_Message* msg);
    DBCPPP_API const char* dbcppp_MessageTransmitter(const dbcppp_Message* msg);
    DBCPPP_API const char* dbcppp_MessageTransmitters_Get(const dbcppp_Message* msg, uint64_t i);
    DBCPPP_API uint64_t dbcppp_MessageTransmitters_Size(const dbcppp_Message* msg);
    DBCPPP_API const dbcppp_Signal* dbcppp_MessageSignals_Get(const dbcppp_Message* msg, uint64_t i);
    DBCPPP_API uint64_t dbcppp_MessageSignals_Size(const dbcppp_Message* msg);
    DBCPPP_API const dbcppp_Attribute* dbcppp_MessageAttributeValues_Get(const dbcppp_Message* msg, uint64_t i);
    DBCPPP_API uint64_t dbcppp_MessageAttributeValues_Size(const dbcppp_Message* msg);
    DBCPPP_API const char* dbcppp_MessageComment(const dbcppp_Message* msg);
    
    DBCPPP_API const dbcppp_Network* dbcppp_NetworkLoadDBCFromFile(const char* filename);
    DBCPPP_API const dbcppp_Network* dbcppp_NetworkLoadDBCFromMemory(const char* data);
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
    DBCPPP_API const char* dbcppp_NetworkVersion(const dbcppp_Network* net);
    DBCPPP_API const char* dbcppp_NetworkNewSymbols_Get(const dbcppp_Network* net, uint64_t i);
    DBCPPP_API uint64_t dbcppp_NetworkNewSymbols_Size(const dbcppp_Network* net);
    DBCPPP_API const dbcppp_BitTiming* dbcppp_NetworkBitTiming(const dbcppp_Network* net);
    DBCPPP_API const dbcppp_Node* dbcppp_NetworkNodes_Get(const dbcppp_Network* net, uint64_t i);
    DBCPPP_API uint64_t dbcppp_NetworkNodes_Size(const dbcppp_Network* net);
    DBCPPP_API const dbcppp_ValueTable* dbcppp_NetworkValueTables_Get(const dbcppp_Network* net, uint64_t i);
    DBCPPP_API uint64_t dbcppp_NetworkValueTables_Size(const dbcppp_Network* net);
    DBCPPP_API const dbcppp_Message* dbcppp_NetworkMessages_Get(const dbcppp_Network* net, uint64_t i);
    DBCPPP_API uint64_t dbcppp_NetworkMessages_Size(const dbcppp_Network* net);    
    DBCPPP_API const dbcppp_EnvironmentVariable* dbcppp_NetworkEnvironmentVariables_Get(const dbcppp_Network* net, uint64_t i);
    DBCPPP_API uint64_t dbcppp_NetworkEnvironmentVariables_Size(const dbcppp_Network* net);
    DBCPPP_API const dbcppp_AttributeDefinition* dbcppp_NetworkAttributeDefinitions_Get(const dbcppp_Network* net, uint64_t i);
    DBCPPP_API uint64_t dbcppp_NetworkAttributeDefinitions_Size(const dbcppp_Network* net);
    DBCPPP_API const dbcppp_Attribute* dbcppp_NetworkAttributeDefaults_Get(const dbcppp_Network* net, uint64_t i);
    DBCPPP_API uint64_t dbcppp_NetworkAttributeDefaults_Size(const dbcppp_Network* net);
    DBCPPP_API const dbcppp_Attribute* dbcppp_NetworkAttributeValues_Get(const dbcppp_Network* net, uint64_t i);
    DBCPPP_API uint64_t dbcppp_NetworkAttributeValues_Size(const dbcppp_Network* net);
    DBCPPP_API const char* dbcppp_NetworkComment(const dbcppp_Network* net);

    DBCPPP_API const dbcppp_Node* dbcppp_NodeCreate(const char* name, const char* comment, dbcppp_Attribute** attributes);
    DBCPPP_API void dbcppp_NodeFree(const dbcppp_Node* node);
    DBCPPP_API const char* dbcppp_NodeName(const dbcppp_Node* node);
    DBCPPP_API const dbcppp_Attribute* dbcppp_NodeAttributeValues_Get(const dbcppp_Network* net, uint64_t i);
    DBCPPP_API uint64_t dbcppp_NodeAttributeValues_Size(const dbcppp_Network* net);
    DBCPPP_API const char* dbcppp_NodeComment(const dbcppp_Node* node);

    DBCPPP_API const dbcppp_Signal* dbcppp_SignalCreate(
          uint64_t message_size
        , const char* name
        , dbcppp_ESignalMultiplexer multiplexer_indicator
        , uint64_t multiplexer_switch_value
        , uint64_t start_bit
        , uint64_t bit_size
        , dbcppp_ESignalByteOrder byte_order
        , dbcppp_ESignalValueType value_type
        , double factor
        , double offset
        , double minimum
        , double maximum
        , const char* unit
        , const char** receivers
        , dbcppp_Attribute** attribute_values
        , dbcppp_ValueEncodingDescription** value_descriptions
        , const char* comment
        , dbcppp_ESignalExtendedValueType extended_value_type);
    DBCPPP_API const char* dbcppp_SignalName(const dbcppp_Signal* sig);
    DBCPPP_API dbcppp_ESignalMultiplexer dbcppp_SignalMultiplexerIndicator(const dbcppp_Signal* sig);
    DBCPPP_API uint64_t dbcppp_SignalMultiplexerSwitchValue(const dbcppp_Signal* sig);
    DBCPPP_API uint64_t dbcppp_SignalMultiplexerValues_Size(const dbcppp_Signal* sig);
    DBCPPP_API const dbcppp_SignalMultiplexerValue* dbcppp_SignalMultiplexerValues_Get(const dbcppp_Signal* sig, uint64_t i);
    DBCPPP_API const char* dbcppp_SignalMultiplexerValue_SwitchName(const dbcppp_SignalMultiplexerValue* mux_val);
    DBCPPP_API uint64_t dbcppp_SignalMultiplexerValue_ValueRanges_Size(const dbcppp_SignalMultiplexerValue* mux_val);
    DBCPPP_API const dbcppp_ValueRange* dbcppp_SignalMultiplexerValue_ValueRanges_Get(const dbcppp_SignalMultiplexerValue* mux_val, uint64_t i);
    DBCPPP_API uint64_t dbcppp_SignalStartBit(const dbcppp_Signal* sig);
    DBCPPP_API uint64_t dbcppp_SignalBitSize(const dbcppp_Signal* sig);
    DBCPPP_API dbcppp_ESignalByteOrder dbcppp_SignalByteOrder(const dbcppp_Signal* sig);
    DBCPPP_API dbcppp_ESignalValueType dbcppp_SignalValueType(const dbcppp_Signal* sig);
    DBCPPP_API double dbcppp_SignalFactor(const dbcppp_Signal* sig);
    DBCPPP_API double dbcppp_SignalOffset(const dbcppp_Signal* sig);
    DBCPPP_API double dbcppp_SignalMinimum(const dbcppp_Signal* sig);
    DBCPPP_API double dbcppp_SignalMaximum(const dbcppp_Signal* sig);
    DBCPPP_API const char* dbcppp_SignalUnit(const dbcppp_Signal* sig);
    DBCPPP_API const char* dbcppp_SignalReceivers_Get(const dbcppp_Signal* signal, uint64_t i);
    DBCPPP_API uint64_t dbcppp_SignalReceivers_Size(const dbcppp_Signal* signal);
    DBCPPP_API const dbcppp_ValueEncodingDescription* dbcppp_SignalValueEncodingDescriptions_Get(const dbcppp_Signal* signal, uint64_t i);
    DBCPPP_API uint64_t dbcppp_SignalValueEncodingDescriptions_Size(const dbcppp_Signal* signal);
    DBCPPP_API const dbcppp_Attribute* dbcppp_SignalAttributeValues_Get(const dbcppp_Signal* signal, uint64_t i);
    DBCPPP_API uint64_t dbcppp_SignalAttributeValues_Size(const dbcppp_Signal* signal);
    DBCPPP_API const char* dbcppp_SignalComment(const dbcppp_Signal* sig);
    DBCPPP_API dbcppp_ESignalExtendedValueType dbcppp_SignalExtended_ValueType(const dbcppp_Signal* sig);
    DBCPPP_API uint64_t dbcppp_SignalDecode(const dbcppp_Signal* sig, const void* bytes);
    DBCPPP_API void dbcppp_SignalEncode(const dbcppp_Signal* sig, uint64_t raw, void* buffer);
    DBCPPP_API double dbcppp_SignalRawToPhys(const dbcppp_Signal* sig, uint64_t raw);
    DBCPPP_API uint64_t dbcppp_SignalPhysToRaw(const dbcppp_Signal* sig, double phys);

    DBCPPP_API const dbcppp_SignalType* dbcppp_SignalTypeCreate(
          const char* name
        , uint64_t signal_size
        , dbcppp_ESignalByteOrder byte_order
        , dbcppp_ESignalValueType value_type
        , double factor
        , double offset
        , double minimum
        , double maximum
        , const char* unit
        , double default_value
        , const char* value_table);
    DBCPPP_API const char* dbcppp_SignalTypeName(const dbcppp_SignalType* sig_type);
    DBCPPP_API uint64_t dbcppp_SignalTypeSignalSize(const dbcppp_SignalType* sig_type);
    DBCPPP_API dbcppp_ESignalByteOrder dbcppp_SignalTypeByteOrder(const dbcppp_SignalType* sig_type);
    DBCPPP_API dbcppp_ESignalValueType dbcppp_SignalTypeValueType(const dbcppp_SignalType* sig_type);
    DBCPPP_API double dbcppp_SignalTypeFactor(const dbcppp_SignalType* sig_type);
    DBCPPP_API double dbcppp_SignalTypeOffset(const dbcppp_SignalType* sig_type);
    DBCPPP_API double dbcppp_SignalTypeMinimum(const dbcppp_SignalType* sig_type);
    DBCPPP_API double dbcppp_SignalTypeMaximum(const dbcppp_SignalType* sig_type);
    DBCPPP_API const char* dbcppp_SignalTypeUnit(const dbcppp_SignalType* sig_type);
    DBCPPP_API double dbcppp_SignalTypeDefaultValue(const dbcppp_SignalType* sig_type);
    DBCPPP_API const char* dbcppp_SignalTypeValueTable(const dbcppp_SignalType* sig_type);
    DBCPPP_API const dbcppp_ValueTable* dbcppp_ValueTableCreate(const char* name, dbcppp_SignalType* signal_type, dbcppp_ValueEncodingDescription** pairs);
    DBCPPP_API void dbcppp_ValueTableFree(const dbcppp_ValueTable* value_table);
    DBCPPP_API const char* dbcppp_ValueTableName(const dbcppp_ValueTable* value_table);
    DBCPPP_API const dbcppp_SignalType* dbcppp_ValueTableSignalType(const dbcppp_ValueTable* value_table);
    DBCPPP_API const dbcppp_ValueEncodingDescription* dbcppp_ValueTableValueEncodingDescription_Get(const dbcppp_ValueTable* value_table, uint64_t i);
    DBCPPP_API uint64_t dbcppp_ValueTableValueEncodingDescription_Size(const dbcppp_ValueTable* value_table);
    
    DBCPPP_API const dbcppp_ValueEncodingDescription* dbcppp_ValueEncodingDescriptionCreate(uint64_t value, const char* desc);
    DBCPPP_API void dbcppp_ValueEncodingDescriptionFree(const dbcppp_ValueEncodingDescription* ved);
    DBCPPP_API uint64_t dbcppp_ValueEncodingDescriptionValue(const dbcppp_ValueEncodingDescription* ved);
    DBCPPP_API const char* dbcppp_ValueEncodingDescriptionDescription(const dbcppp_ValueEncodingDescription* ved);

#ifdef __cplusplus
}
#endif
