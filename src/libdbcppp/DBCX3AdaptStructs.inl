BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_Version,
    version
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_NewSymbols,
    new_symbols
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_BitTiming,
    baudrate, BTR1, BTR2
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_Node,
    name
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_ValueTable,
    name, value_encoding_descriptions
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_Signal,
    name, multiplexer_indicator,
    start_bit, signal_size,
    byte_order, value_type,
    factor, offset, minimum,
    maximum, unit, receivers
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_Message,
    id, name, size,
    transmitter, signals
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_MessageTransmitter,
    id, transmitters
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_EnvironmentVariable,
    name, var_type, minimum,
    maximum, unit, initial_value,
    id, access_type, access_nodes
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_EnvironmentVariableData,
    name, size
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_SignalType,
    name, size, byte_order, value_type,
    factor, offset, minimum, maximum,
    unit, default_value, value_table_name
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_CommentNetwork,
    comment
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_CommentNode,
    node_name, comment
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_CommentMessage,
    message_id, comment
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_CommentSignal,
    message_id, signal_name, comment
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_CommentEnvVar,
    env_var_name, comment
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_Comment,
    comment
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_AttributeValueTypeInt,
    minimum, maximum
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_AttributeValueTypeHex,
    minimum, maximum
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_AttributeValueTypeFloat,
    minimum, maximum
)
// Since G_AttributeValueTypeString has no attributes the struct must not be adapted
//BOOST_FUSION_ADAPT_STRUCT(
//    dbcppp::DBCX3::AST::G_AttributeValueTypeString,
//)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_AttributeValueTypeEnum,
    values
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_AttributeValue,
    value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_AttributeDefinition,
    object_type, name, value_type
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_Attribute,
    name, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_AttributeNetwork,
    attribute_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_AttributeNode,
    attribute_name, node_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_AttributeMessage,
    attribute_name, message_id, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_AttributeSignal,
    attribute_name, message_id,
    signal_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_AttributeEnvVar,
    attribute_name, env_var_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_ValueDescriptionSignal,
    message_id, signal_name, value_descriptions
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_ValueDescriptionEnvVar,
    env_var_name, value_descriptions
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_ValueDescription,
    description
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_SignalExtendedValueType,
    message_id, signal_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::DBCX3::AST::G_Network,
    version, new_symbols, bit_timing,
    nodes, value_tables, messages,
    message_transmitters,
    environment_variables,
    environment_variable_datas,
    signal_types, comments,
    attribute_definitions,
    attribute_defaults,
    attribute_values,
    value_descriptions,
    signal_extended_value_types
)