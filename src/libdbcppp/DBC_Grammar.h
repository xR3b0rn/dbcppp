
#pragma once

#include <map>
#include <string>
#include <vector>
#include <istream>
#include <cstdint>
#include <ostream>
#include <fstream>
#include <iterator>
#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/phoenix.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/qi_hold.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/repository/include/qi_iter_pos.hpp>

#include "../../include/dbcppp/CApi.h"
#include "DBC_Grammar.h"
#include "NetworkImpl.h"

namespace dbcppp
{
    using variant_attr_value_t = boost::variant<int64_t, double, std::string>;

    struct G_Version
    {
        std::string::iterator position;
        std::string version;
    };
    struct G_NewSymbols
    {
        std::string::iterator position;
        std::vector<std::string> new_symbols;
    };
    struct G_BitTiming
    {
        std::string::iterator position;
        uint64_t baudrate;
        uint64_t BTR1;
        uint64_t BTR2;
    };
    struct G_Node
    {
        std::string::iterator position;
        std::string name;
    };
    struct G_ValueTable
    {
        std::string::iterator position;
        std::string name;
        std::map<int64_t, std::string> value_encoding_descriptions;
    };
    struct G_Signal
    {
        std::string::iterator position;
        std::string name;
        boost::optional<std::string> multiplexer_indicator;
        uint64_t start_bit;
        uint64_t signal_size;
        char byte_order;
        char value_type;
        double factor;
        double offset;
        double minimum;
        double maximum;
        std::string unit;
        std::vector<std::string> receivers;
    };
    struct G_Message
    {
        std::string::iterator position;
        uint64_t id;
        std::string name;
        uint64_t size;
        std::string transmitter;
        std::vector<G_Signal> signals;
    };
    struct G_MessageTransmitter
    {
        std::string::iterator position;
        uint64_t id;
        std::vector<std::string> transmitters;
    };
    struct G_EnvironmentVariable
    {
        std::string::iterator position;
        std::string name;
        uint64_t var_type;
        double minimum;
        double maximum;
        std::string unit;
        double initial_value;
        uint64_t id;
        std::string access_type;
        std::vector<std::string> access_nodes;
    };
    struct G_EnvironmentVariableData
    {
        std::string::iterator position;
        std::string name;
        uint64_t size;
    };
    struct G_SignalType
    {
        std::string::iterator position;
        std::string name;
        uint64_t size;
        char byte_order;
        char value_type;
        double factor;
        double offset;
        double minimum;
        double maximum;
        std::string unit;
        double default_value;
        std::string value_table_name;
    };
    struct G_CommentNetwork
    {
        std::string::iterator position;
        std::string comment;
    };
    struct G_CommentNode
    {
        std::string::iterator position;
        std::string node_name;
        std::string comment;
    };
    struct G_CommentMessage
    {
        std::string::iterator position;
        uint64_t message_id;
        std::string comment;
    };
    struct G_CommentSignal
    {
        std::string::iterator position;
        uint64_t message_id;
        std::string signal_name;
        std::string comment;
    };
    struct G_CommentEnvVar
    {
        std::string::iterator position;
        std::string env_var_name;
        std::string comment;
    };
    using variant_comment_t = boost::variant<G_CommentNetwork, G_CommentNode, G_CommentMessage, G_CommentSignal, G_CommentEnvVar>;
    struct G_Comment
    {
        std::string::iterator position;
        variant_comment_t comment;
    };
    struct G_AttributeValueTypeInt
    {
        std::string::iterator position;
        int64_t minimum;
        int64_t maximum;
    };
    struct G_AttributeValueTypeHex
    {
        std::string::iterator position;
        int64_t minimum;
        int64_t maximum;
    };
    struct G_AttributeValueTypeFloat
    {
        std::string::iterator position;
        double minimum;
        double maximum;
    };
    struct G_AttributeValueTypeString
    {
        std::string::iterator position;
    };
    struct G_AttributeValueTypeEnum
    {
        std::string::iterator position;
        std::vector<std::string> values;
    };
    using variant_attribute_value_t =
        boost::variant<
            G_AttributeValueTypeInt, G_AttributeValueTypeHex,
            G_AttributeValueTypeFloat, G_AttributeValueTypeString,
            G_AttributeValueTypeEnum>;
    struct G_AttributeValue
    {
        std::string::iterator position;
        variant_attribute_value_t value;
    };
    struct G_AttributeDefinition
    {
        std::string::iterator position;
        boost::optional<std::string> object_type;
        std::string name;
        G_AttributeValue value_type;
    };
    struct G_Attribute
    {
        std::string::iterator position;
        std::string name;
        variant_attr_value_t value;
    };
    struct G_AttributeNetwork
    {
        std::string::iterator position;
        std::string attribute_name;
        variant_attr_value_t value;
    };
    struct G_AttributeNode
    {
        std::string::iterator position;
        std::string attribute_name;
        std::string node_name;
        variant_attr_value_t value;
    };
    struct G_AttributeMessage
    {
        std::string::iterator position;
        std::string attribute_name;
        uint64_t message_id;
        variant_attr_value_t value;
    };
    struct G_AttributeSignal
    {
        std::string::iterator position;
        std::string attribute_name;
        uint64_t message_id;
        std::string signal_name;
        variant_attr_value_t value;
    };
    struct G_AttributeEnvVar
    {
        std::string::iterator position;
        std::string attribute_name;
        std::string env_var_name;
        variant_attr_value_t value;
    };
    using variant_attribute_t = boost::variant<G_AttributeNetwork, G_AttributeNode, G_AttributeMessage, G_AttributeSignal, G_AttributeEnvVar>;
    struct G_ValueDescriptionSignal
    {
        std::string::iterator position;
        uint64_t message_id;
        std::string signal_name;
        std::map<int64_t, std::string> value_descriptions;
    };
    struct G_ValueDescriptionEnvVar
    {
        std::string::iterator position;
        std::string env_var_name;
        std::map<int64_t, std::string> value_descriptions;
    };
    struct G_ValueDescription
    {
        std::string::iterator position;
        boost::variant<G_ValueDescriptionSignal, G_ValueDescriptionEnvVar> description;
    };
    struct G_SignalExtendedValueType
    {
        std::string::iterator position;
        uint64_t message_id;
        std::string signal_name;
        uint64_t value;
    };
    struct G_Network
    {
        std::string::iterator position;
        G_Version version;
        std::vector<std::string> new_symbols;
        boost::optional<G_BitTiming> bit_timing;
        std::vector<G_Node> nodes;
        std::vector<G_ValueTable> value_tables;
        std::vector<G_Message> messages;
        std::vector<G_MessageTransmitter> message_transmitters;
        std::vector<G_EnvironmentVariable> environment_variables;
        std::vector<G_EnvironmentVariableData> environment_variable_datas;
        std::vector<G_SignalType> signal_types;
        std::vector<variant_comment_t> comments;
        std::vector<G_AttributeDefinition> attribute_definitions;
        std::vector<G_Attribute> attribute_defaults;
        std::vector<variant_attribute_t> attribute_values;
        std::vector<G_ValueDescription> value_descriptions;
        std::vector<G_SignalExtendedValueType> signal_extended_value_types;
    };
}
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_Version,
    position,
    version
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_NewSymbols,
    position,
    new_symbols
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_BitTiming,
    position,
    baudrate, BTR1, BTR2
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_Node,
    position,
    name
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_ValueTable,
    position,
    name, value_encoding_descriptions
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_Signal,
    position,
    name, multiplexer_indicator,
    start_bit, signal_size,
    byte_order, value_type,
    factor, offset, minimum,
    maximum, unit, receivers
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_Message,
    position,
    id, name, size,
    transmitter, signals
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_MessageTransmitter,
    position,
    id, transmitters
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_EnvironmentVariable,
    position,
    name, var_type, minimum,
    maximum, unit, initial_value,
    id, access_type, access_nodes
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_EnvironmentVariableData,
    position,
    name, size
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_SignalType,
    position,
    name, size, byte_order, value_type,
    factor, offset, minimum, maximum,
    unit, default_value, value_table_name
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_CommentNetwork,
    position,
    comment
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_CommentNode,
    position,
    node_name, comment
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_CommentMessage,
    position,
    message_id, comment
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_CommentSignal,
    position,
    message_id, signal_name, comment
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_CommentEnvVar,
    position,
    env_var_name, comment
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_Comment,
    position,
    comment
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_AttributeValueTypeInt,
    position,
    minimum, maximum
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_AttributeValueTypeHex,
    position,
    minimum, maximum
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_AttributeValueTypeFloat,
    position,
    minimum, maximum
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_AttributeValueTypeString,
    position
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_AttributeValueTypeEnum,
    position,
    values
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_AttributeValue,
    position,
    value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_AttributeDefinition,
    position,
    object_type, name, value_type
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_Attribute,
    position,
    name, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_AttributeNetwork,
    position,
    attribute_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_AttributeNode,
    position,
    attribute_name, node_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_AttributeMessage,
    position,
    attribute_name, message_id, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_AttributeSignal,
    position,
    attribute_name, message_id,
    signal_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_AttributeEnvVar,
    position,
    attribute_name, env_var_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_ValueDescriptionSignal,
    position,
    message_id, signal_name, value_descriptions
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_ValueDescriptionEnvVar,
    position,
    env_var_name, value_descriptions
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_ValueDescription,
    position,
    description
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_SignalExtendedValueType,
    position,
    message_id, signal_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
    dbcppp::G_Network,
    position,
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

namespace dbcppp
{
    template <class Iter>
    std::tuple<std::size_t, std::size_t> getErrPos(Iter iter, Iter cur)
    {
        std::size_t line = 1;
        std::size_t column = 1;
        while (iter != cur)
        {
            if (*iter == '\n')
            {
                line++;
                column = 1;
            }
            else
            {
                column++;
            }
            iter++;
        }
        return {line, column};
    }


    template <class Iter>
    class NetworkGrammar
        : public boost::spirit::qi::grammar<Iter, G_Network(), boost::spirit::ascii::space_type>
    {
    public:
        using Skipper = boost::spirit::ascii::space_type;

        NetworkGrammar(Iter begin)
            : NetworkGrammar::base_type(_network, "DBC_Network")
        {
            namespace fu = boost::fusion;
            namespace ph = boost::phoenix;
            namespace qi = boost::spirit::qi;
            namespace sp = boost::spirit::labels;
            namespace ascii = boost::spirit::ascii;

            _unsigned_integer.name("uint");
            _signed_integer.name("int");
            _double.name("double");
            _char_string.name("QuotedString");
            _C_identifier.name("Identifier");
            _C_identifier_.name("Identifier");
            _version.name("Version");
            _new_symbol.name("NewSymbol");
            _new_symbols.name("NS_");
            _bit_timing.name("BitTiming");
            _baudrate.name("Baudrate");
            _BTR1.name("BTR1");
            _BTR2.name("BTR2");
            _nodes.name("Nodes");
            _node_.name("Node");
            _node_name.name("NodeName");
            _node_name.name("NodeName");
            _value_tables.name("ValueTables");
            _value_table.name("ValueTable");
            _value_table_name.name("ValueTableName");
            _value_encoding_descriptions.name("VAlueEncodingDescriptions");
            _value_encoding_description.name("VAlueEncodingDescription");
            _messages.name("Messages");
            _message.name("Message");
            _message_id.name("MessageId");
            _message_name.name("MessageName");
            _message_size.name("MessageSize");
            _transmitter.name("NodeName");
            _transmitter_.name("NodeName");
            _signals.name("Signals");
            _signal.name("Signal");
            _signal_name.name("SignalName");
            _multiplexer_indicator.name("SignalMultiplexerIndicator");
            _attribute_value_type_string.name("AttributeValueTypeString");
            _start_bit.name("SignalStartBit");
            _start_bit %= _unsigned_integer;
            _signal_size.name("SignalSize");
            _signal_size %= _unsigned_integer;
            _byte_order.name("ByteOrder");
            _byte_order = qi::lexeme[qi::char_('0') | qi::char_('1')];
            _value_type.name("ValueType");
            _value_type = qi::lexeme[qi::char_('-') | qi::char_('+')];
            _factor.name("Factor");
            _factor %= _double;
            _offset.name("Offset");
            _offset %= _double;
            _minimum.name("Minimum");
            _minimum %= _double;
            _maximum.name("Maximum");
            _unit.name("Unit");
            _receivers.name("NodeNames");
            _receiver_.name("NodeName");
            _receivers_.name("NodeNames");
            _message_transmitters.name("MessageTransmitters");
            _message_transmitter.name("MessageTransmitter");
            _transmitters.name("NodeNames");
            _environment_variables.name("EnvironmentVariables");
            _environment_variable.name("EnvironmentVariable");
            _env_var_name.name("EnvVarName");
            _env_var_type.name("EnvVarType");
            _initial_value.name("InitialValue");
            _ev_id.name("EvId");
            _access_type.name("AccessType");
            _access_nodes.name("NodeNames");
            _environment_variable_datas.name("EnvironmentVariableDatas");
            _environment_variable_data.name("EnvironmentVariableData");
            _data_size.name("DataSize");
            _signal_types.name("SignalTypes");
            _signal_type.name("SignalType");
            _signal_type_name.name("SignalTypeName");
            _default_value.name("DefaultValue");
            _comments.name("Comments");
            _comment.name("Comment");
            _comment_network.name("CommentNetwork");
            _comment_node.name("CommentNode");
            _comment_message.name("CommentMessage");
            _comment_signal.name("CommentSignal");
            _comment_env_var.name("CommentEnvVar");
            _attribute_definitions.name("AttributeDefinitions");
            _attribute_definition.name("AttributeDefinition");
            _object_type.name("ObjectType");
            _attribute_name.name("AttributeName");
            _attribute_value_type.name("AttributeValueType");
            _attribute_value_type_int.name("AttributeValueTypeInt");
            _attribute_value_type_hex.name("AttributeValueTypeHex");
            _attribute_value_type_float.name("AttributeValueTypeFloat");
            _attribute_value_type_enum.name("AttributeValueTypeEnum");
            _attribute_defaults.name("AttributeDefaults");
            _attribute_default.name("AttributeDefault");
            _attribute_value.name("AttributeValue");
            _attribute_values.name("AttributeValues");
            _attribute_value_ent.name("AttributeValueEnt");
            _attribute_value_ent_network.name("AttributeValueEntNetwork");
            _attribute_value_ent_node.name("AttributeValueEntNode");
            _attribute_value_ent_message.name("AttributeValueEntMessage");
            _attribute_value_ent_signal.name("AttributeValueEntSignal");
            _attribute_value_ent_env_var.name("AttributeValueEntEnvVar");
            _value_descriptions.name("ValueDescriptions");
            _value_description_sig_env_var.name("ValueDescriptionsSigEnvVar");
            _value_description_signal.name("ValueDescriptionsSignal");
            _value_description_env_var.name("ValueDescriptionsEnvVar");
            _signal_extended_value_types.name("SignalExtendedValueTypes");
            _signal_extended_value_type.name("SignalExtendedValueType");


            using boost::spirit::repository::qi::iter_pos;

            _network.name("Network");
            _network %=
                   iter_pos
                > _version
                > _new_symbols
                > _bit_timing
                > _nodes
                > _value_tables
                > _messages
                > _message_transmitters
                > _environment_variables
                > _environment_variable_datas
                > _signal_types
                > _comments
                > _attribute_definitions
                > _attribute_defaults
                > _attribute_values

                > _value_descriptions
                > _signal_extended_value_types
                ;

            _unsigned_integer %= qi::uint_;
            _signed_integer %= qi::int_;
            _double %= qi::double_;
            _char_string %= qi::lexeme['"' >> *(('\\' >> qi::char_("\\\"")) | ~qi::char_('"')) >> '"'];
            _C_identifier %= qi::lexeme[qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9")];
            _C_identifier_ %= qi::lexeme[qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9")];

            _version %= iter_pos >> qi::lit("VERSION") > _char_string;
        
            _new_symbol %=
                qi::lexeme[
                      qi::string("SIGTYPE_VALTYPE_")
                    | qi::string("BA_DEF_DEF_REL_")
                    | qi::string("BA_DEF_SGTYPE_")
                    | qi::string("SIG_TYPE_REF_")
                    | qi::string("ENVVAR_DATA_")
                    | qi::string("SIG_VALTYPE_")
                    | qi::string("SG_MUL_VAL_")
                    | qi::string("BA_DEF_DEF_")
                    | qi::string("ENVVAR_DTA_")
                    | qi::string("BA_DEF_REL_")
                    | qi::string("SGTYPE_VAL_")
                    | qi::string("VAL_TABLE_")
                    | qi::string("BA_SGTYPE_")
                    | qi::string("SIG_GROUP_")
                    | qi::string("BU_SG_REL_")
                    | qi::string("BU_EV_REL_")
                    | qi::string("BU_BO_REL_")
                    | qi::string("BO_TX_BU_")
                    | qi::string("NS_DESC_")
                    | qi::string("CAT_DEF_")
                    | qi::string("EV_DATA_")
                    | qi::string("BA_REL_")
                    | qi::string("SGTYPE_")
                    | qi::string("BA_DEF_")
                    | qi::string("FILTER")
                    | qi::string("DEF_")
                    | qi::string("VAL_")
                    | qi::string("CAT_")
                    | qi::string("BA_")
                    | qi::string("CM_")
                ];
        
            _new_symbols %= qi::lit("NS_") > ':' > *_new_symbol;

            _bit_timing %= qi::lit("BS_") > ':' >> -_bit_timing_inner;
            _bit_timing_inner %= iter_pos >> _baudrate >> ':' >> _BTR1 >> ',' >> _BTR2;
            _baudrate %= _unsigned_integer;
            _BTR1 %= _unsigned_integer;
            _BTR2 %= _unsigned_integer;
        
            _nodes %= qi::lit("BU_") > ':' > qi::skip(ascii::blank)[*_node_ > qi::eol];
            _node_ %= iter_pos >> _node_name_;
            _node_name %= _C_identifier;
            _node_name_ %= _C_identifier_;

            _value_tables %= *_value_table;
            _value_table %= iter_pos >> qi::lit("VAL_TABLE_") > _value_table_name > _value_encoding_descriptions > ';';
            _value_table_name %= _C_identifier;
            _value_encoding_descriptions %= *_value_encoding_description;

            _value_encoding_description %= _signed_integer > _char_string;

            _messages %= *_message;
            _message %= iter_pos >> qi::lit("BO_ ") > _message_id > _message_name
                > ':' > _message_size > qi::skip(ascii::blank)[_transmitter_ > qi::eol] > _signals;
            _message_id %= _unsigned_integer;
            _message_name %= _C_identifier;
            _message_size %= _unsigned_integer;
            _transmitter %= _node_name;
            _transmitter_ %= _node_name_;

            _signals %= *_signal;
            _signal %= iter_pos >> qi::lexeme[qi::lit("SG_") >> qi::omit[qi::space]] > _signal_name
                > _multiplexer_indicator > ':' > _start_bit > '|' > _signal_size > '@' > _byte_order > _value_type
                > '(' > _factor > ',' > _offset > ')' > '[' > _minimum > '|' > _maximum > ']' > _unit
                > qi::skip(ascii::blank)[_receivers_ > qi::eol];
            _signal_name %= _C_identifier;
            _multiplexer_indicator %= -_C_identifier;
            _maximum %= _double;
            _unit %= _char_string;
            _receivers %= qi::skip(ascii::blank)[*_receiver_ > qi::eol];
            _receiver_ %= _node_name_;
            _receivers_ %= _receiver_ % ',';

            _message_transmitters %= *_message_transmitter;
            _message_transmitter %= iter_pos >> qi::lexeme[qi::lit("BO_TX_BU_") >> qi::omit[qi::space]] > _message_id > ':' > _transmitters > ';';
            _transmitters %= _transmitter % ',';

            _environment_variables %= *_environment_variable;
            _environment_variable %= iter_pos >> qi::lexeme[qi::lit("EV_") >> qi::omit[qi::space]] > _env_var_name > ':' > _env_var_type
                > '[' > _minimum > '|' > _maximum > ']' > _unit > _initial_value > _ev_id > _access_type > _access_nodes > ';';
            _env_var_name %= _C_identifier;
            _env_var_type %= _unsigned_integer;
            _initial_value %= _double;
            _ev_id %= _unsigned_integer;
            _access_type %= _C_identifier;
            _access_nodes %= _node_name % ',';

            _environment_variable_datas %= *_environment_variable_data;
            _environment_variable_data %= iter_pos >> qi::lexeme[qi::lit("ENVVAR_DATA_") >> qi::omit[qi::space]]
                > _env_var_name > ':' > _data_size > ';';
            _data_size %= _unsigned_integer;

            _signal_types %= *_signal_type;
            _signal_type %= iter_pos >> qi::lexeme[qi::lit("SGTYPE_") >> qi::omit[qi::space]] > _signal_type_name > ':' > _signal_size > '@'
                > _byte_order > _value_type > '(' > _factor > ',' > _offset > ')' > '['
                > _minimum > '|' >> _maximum > ']' > _unit > _default_value > ',' > _value_table_name > ';';
            _signal_type_name %= _C_identifier;
            _default_value %= _double;
        
            _comments %= *_comment;
            _comment %= qi::lexeme[qi::lit("CM_") >> qi::omit[qi::space]]
                > (_comment_node | _comment_message | _comment_signal | _comment_env_var | _comment_network);
            _comment_network %= iter_pos >> _char_string > ';';
            _comment_node %= iter_pos >> qi::lit("BU_") > _node_name > _char_string > ';';
            _comment_message %= iter_pos >> qi::lit("BO_") > _message_id > _char_string > ';';
            _comment_signal %= iter_pos >> qi::lit("SG_") > _message_id > _signal_name > _char_string > ';';
            _comment_env_var %= iter_pos >> qi::lit("EV_") > _env_var_name > _char_string > ';';

            _attribute_definitions %= *_attribute_definition;
            _attribute_definition %= iter_pos >> qi::lexeme[qi::lit("BA_DEF_") >> qi::omit[qi::space]] > _object_type > _attribute_name
                > _attribute_value_type > ';';
            _object_type %= -(qi::string("BU_") | qi::string("BO_") | qi::string("SG_") | qi::string("EV_"));
            _attribute_name %= _char_string;
            _attribute_value_type %= iter_pos >>
                  (_attribute_value_type_int | _attribute_value_type_hex
                | _attribute_value_type_float | _attribute_value_type_string
                | _attribute_value_type_enum);
            _attribute_value_type_int %= iter_pos >> qi::lit("INT") > _signed_integer > _signed_integer;
            _attribute_value_type_hex %= iter_pos >> qi::lit("HEX") > _signed_integer > _signed_integer;
            _attribute_value_type_float %= iter_pos >> qi::lit("FLOAT") > _double > _double;
            _attribute_value_type_string %= iter_pos >> qi::lit("STRING");
            _attribute_value_type_enum %= iter_pos >> qi::lit("ENUM") > (_char_string % ',');
        
            _attribute_defaults %= *_attribute_default;
            _attribute_default %= iter_pos >> (qi::lexeme[(qi::lit("BA_DEF_DEF_REL_") | qi::lit("BA_DEF_DEF_"))  >> qi::omit[qi::space]])
                > _attribute_name > _attribute_value > ';';
            _attribute_value %= _double | _signed_integer | _char_string;

            _attribute_values %= *_attribute_value_ent;
            _attribute_value_ent %= qi::lexeme[qi::lit("BA_") >> qi::omit[qi::space]]
                >> (_attribute_value_ent_network | _attribute_value_ent_node
                  | _attribute_value_ent_message | _attribute_value_ent_signal
                  | _attribute_value_ent_env_var) > ';';
            _attribute_value_ent_network %= iter_pos >> _attribute_name >> _attribute_value;
            _attribute_value_ent_node %= iter_pos >> _attribute_name >> qi::lit("BU_") >> _node_name >> _attribute_value;
            _attribute_value_ent_message %= iter_pos >> _attribute_name >> qi::lit("BO_") >> _message_id >> _attribute_value;
            _attribute_value_ent_signal %= iter_pos >> _attribute_name >> qi::lit("SG_") >> _message_id >> _signal_name >> _attribute_value;
            _attribute_value_ent_env_var %= iter_pos >> _attribute_name >> qi::lit("EV_") >> _env_var_name >> _attribute_value;

            _value_descriptions %= *_value_description_sig_env_var;
            _value_description_sig_env_var %= iter_pos >> (_value_description_signal | _value_description_env_var);
            _value_description_signal %= iter_pos >> qi::lexeme[qi::lit("VAL_") >> qi::omit[qi::space]] >> _message_id
                >> _signal_name >> _value_encoding_descriptions > ';';
            _value_description_env_var %= iter_pos >> qi::lexeme[qi::lit("VAL_") >> qi::omit[qi::space]] >> _env_var_name
                >> _value_encoding_descriptions > ';';

            _signal_extended_value_types %= *_signal_extended_value_type;
            _signal_extended_value_type %= iter_pos >> qi::lexeme[qi::lit("SIG_VALTYPE_") >> qi::omit[qi::space]]
                > _message_id > _signal_name > ':' >  _unsigned_integer > ';';
        
            auto error_handler =
                [begin](const auto& args, const auto& context, const auto& error)
                {
                    const auto& first = fu::at_c<0>(args);
                    const auto& last = fu::at_c<1>(args);
                    const auto& err = fu::at_c<2>(args);
                    const auto& what = fu::at_c<3>(args);
                    auto[line, column] = getErrPos(begin, err);
                    std::cout << line << ":" << column << " Error! Expecting " << what << std::endl;
                };
            qi::on_error<qi::fail>(_network, error_handler);
        }

    private:
        boost::spirit::qi::rule<Iter, G_Network(), Skipper> _network;
    
        boost::spirit::qi::rule<Iter, uint64_t(), Skipper> _unsigned_integer;
        boost::spirit::qi::rule<Iter, int64_t(), Skipper> _signed_integer;
        boost::spirit::qi::rule<Iter, double(), Skipper> _double;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _char_string;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _C_identifier;
        boost::spirit::qi::rule<Iter, std::string(), boost::spirit::ascii::blank_type> _C_identifier_;
    
        boost::spirit::qi::rule<Iter, G_Version, Skipper> _version;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _new_symbol;
        boost::spirit::qi::rule<Iter, std::vector<std::string>(), Skipper> _new_symbols;
        boost::spirit::qi::rule<Iter, boost::optional<G_BitTiming>, Skipper> _bit_timing;
        boost::spirit::qi::rule<Iter, G_BitTiming, Skipper> _bit_timing_inner;
        boost::spirit::qi::rule<Iter, uint64_t(), Skipper> _baudrate;
        boost::spirit::qi::rule<Iter, uint64_t(), Skipper> _BTR1;
        boost::spirit::qi::rule<Iter, uint64_t(), Skipper> _BTR2;

        boost::spirit::qi::rule<Iter, std::vector<G_Node>(), Skipper> _nodes;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _node_name;
        boost::spirit::qi::rule<Iter, G_Node(), boost::spirit::ascii::blank_type> _node_;
        boost::spirit::qi::rule<Iter, std::vector<std::string>(), boost::spirit::ascii::blank_type> _node_names;
        boost::spirit::qi::rule<Iter, std::string(), boost::spirit::ascii::blank_type> _node_name_;

        boost::spirit::qi::rule<Iter, std::vector<G_ValueTable>(), Skipper> _value_tables;
        boost::spirit::qi::rule<Iter, G_ValueTable(), Skipper> _value_table;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _value_table_name;
        boost::spirit::qi::rule<Iter, std::map<int64_t, std::string>(), Skipper> _value_encoding_descriptions;
        boost::spirit::qi::rule<Iter, std::pair<int64_t, std::string>(), Skipper> _value_encoding_description;

        boost::spirit::qi::rule<Iter, std::vector<G_Message>(), Skipper> _messages;
        boost::spirit::qi::rule<Iter, G_Message(), Skipper> _message;
        boost::spirit::qi::rule<Iter, uint64_t(), Skipper> _message_id;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _message_name;
        boost::spirit::qi::rule<Iter, uint64_t(), Skipper> _message_size;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _transmitter;
        boost::spirit::qi::rule<Iter, std::string(), boost::spirit::ascii::blank_type> _transmitter_;
    
        boost::spirit::qi::rule<Iter, std::vector<G_Signal>(), Skipper> _signals;
        boost::spirit::qi::rule<Iter, G_Signal(), Skipper> _signal;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _signal_name;
        boost::spirit::qi::rule<Iter, boost::optional<std::string>(), Skipper> _multiplexer_indicator;
        boost::spirit::qi::rule<Iter, uint64_t(), Skipper> _start_bit;
        boost::spirit::qi::rule<Iter, uint64_t(), Skipper> _signal_size;
        boost::spirit::qi::rule<Iter, char(), Skipper> _byte_order;
        boost::spirit::qi::rule<Iter, char(), Skipper> _value_type;
        boost::spirit::qi::rule<Iter, double(), Skipper> _factor;
        boost::spirit::qi::rule<Iter, double(), Skipper> _offset;
        boost::spirit::qi::rule<Iter, double(), Skipper> _maximum;
        boost::spirit::qi::rule<Iter, double(), Skipper> _minimum;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _unit;
        boost::spirit::qi::rule<Iter, std::vector<std::string>(), Skipper> _receivers;
        boost::spirit::qi::rule<Iter, std::vector<std::string>(), boost::spirit::ascii::blank_type> _receivers_;
        boost::spirit::qi::rule<Iter, std::string(), boost::spirit::ascii::blank_type> _receiver_;

        boost::spirit::qi::rule<Iter, std::vector<G_MessageTransmitter>(), Skipper> _message_transmitters;
        boost::spirit::qi::rule<Iter, G_MessageTransmitter(), Skipper> _message_transmitter;
        boost::spirit::qi::rule<Iter, std::vector<std::string>(), Skipper> _transmitters;

        boost::spirit::qi::rule<Iter, std::vector<G_EnvironmentVariable>(), Skipper> _environment_variables;
        boost::spirit::qi::rule<Iter, G_EnvironmentVariable(), Skipper> _environment_variable;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _env_var_name;
        boost::spirit::qi::rule<Iter, uint64_t(), Skipper> _env_var_type;
        boost::spirit::qi::rule<Iter, double(), Skipper> _initial_value;
        boost::spirit::qi::rule<Iter, uint64_t(), Skipper> _ev_id;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _access_type;
        boost::spirit::qi::rule<Iter, std::vector<std::string>(), Skipper> _access_nodes;
    
        boost::spirit::qi::rule<Iter, std::vector<G_EnvironmentVariableData>(), Skipper> _environment_variable_datas;
        boost::spirit::qi::rule<Iter, G_EnvironmentVariableData(), Skipper> _environment_variable_data;
        boost::spirit::qi::rule<Iter, uint64_t(), Skipper> _data_size;
    
        boost::spirit::qi::rule<Iter, std::vector<G_SignalType>(), Skipper> _signal_types;
        boost::spirit::qi::rule<Iter, G_SignalType(), Skipper> _signal_type;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _signal_type_name;
        boost::spirit::qi::rule<Iter, double(), Skipper> _default_value;

        boost::spirit::qi::rule<Iter, std::vector<variant_comment_t>(), Skipper> _comments;
        boost::spirit::qi::rule<Iter, variant_comment_t(), Skipper> _comment;
        boost::spirit::qi::rule<Iter, G_CommentNetwork(), Skipper> _comment_network;
        boost::spirit::qi::rule<Iter, G_CommentNode(), Skipper> _comment_node;
        boost::spirit::qi::rule<Iter, G_CommentMessage(), Skipper> _comment_message;
        boost::spirit::qi::rule<Iter, G_CommentSignal(), Skipper> _comment_signal;
        boost::spirit::qi::rule<Iter, G_CommentEnvVar(), Skipper> _comment_env_var;

        boost::spirit::qi::rule<Iter, std::vector<G_AttributeDefinition>(), Skipper> _attribute_definitions;
        boost::spirit::qi::rule<Iter, G_AttributeDefinition(), Skipper> _attribute_definition;
        boost::spirit::qi::rule<Iter, boost::optional<std::string>(), Skipper> _object_type;
        boost::spirit::qi::rule<Iter, std::string(), Skipper> _attribute_name;
        boost::spirit::qi::rule<Iter, G_AttributeValue(), Skipper> _attribute_value_type;
        boost::spirit::qi::rule<Iter, G_AttributeValueTypeInt(), Skipper> _attribute_value_type_int;
        boost::spirit::qi::rule<Iter, G_AttributeValueTypeHex(), Skipper> _attribute_value_type_hex;
        boost::spirit::qi::rule<Iter, G_AttributeValueTypeFloat(), Skipper> _attribute_value_type_float;
        boost::spirit::qi::rule<Iter, G_AttributeValueTypeString(), Skipper> _attribute_value_type_string;
        boost::spirit::qi::rule<Iter, G_AttributeValueTypeEnum(), Skipper> _attribute_value_type_enum;

        boost::spirit::qi::rule<Iter, std::vector<G_Attribute>(), Skipper> _attribute_defaults;
        boost::spirit::qi::rule<Iter, G_Attribute(), Skipper> _attribute_default;
        boost::spirit::qi::rule<Iter, variant_attr_value_t(), Skipper> _attribute_value;

        boost::spirit::qi::rule<Iter, std::vector<variant_attribute_t>(), Skipper> _attribute_values;
        boost::spirit::qi::rule<Iter, variant_attribute_t(), Skipper> _attribute_value_ent;
        boost::spirit::qi::rule<Iter, G_AttributeNetwork(), Skipper> _attribute_value_ent_network;
        boost::spirit::qi::rule<Iter, G_AttributeNode(), Skipper> _attribute_value_ent_node;
        boost::spirit::qi::rule<Iter, G_AttributeMessage(), Skipper> _attribute_value_ent_message;
        boost::spirit::qi::rule<Iter, G_AttributeMessage(), Skipper> _attribute_value_ent_message2;
        boost::spirit::qi::rule<Iter, G_AttributeSignal(), Skipper> _attribute_value_ent_signal;
        boost::spirit::qi::rule<Iter, G_AttributeEnvVar(), Skipper> _attribute_value_ent_env_var;

        boost::spirit::qi::rule<Iter, std::vector<G_ValueDescription>(), Skipper> _value_descriptions;
        boost::spirit::qi::rule<Iter, G_ValueDescription(), Skipper> _value_description_sig_env_var;
        boost::spirit::qi::rule<Iter, G_ValueDescriptionSignal(), Skipper> _value_description_signal;
        boost::spirit::qi::rule<Iter, G_ValueDescriptionEnvVar(), Skipper> _value_description_env_var;

        boost::spirit::qi::rule<Iter, std::vector<G_SignalExtendedValueType>(), Skipper> _signal_extended_value_types;
        boost::spirit::qi::rule<Iter, G_SignalExtendedValueType(), Skipper> _signal_extended_value_type;
    };
}
