
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include "NetworkImpl.h"
#include "DBCX3.h"

#include "DBCX3AdaptStructs.inl"

namespace dbcppp::DBCX3::Grammar
{
    using namespace dbcppp::DBCX3::AST;
    using namespace boost::spirit::x3;
    
    struct G_Test {};

    static const rule<class BlockComment> block_comment("BlockComment");
    static const rule<class CharString, std::string> quoted_string("CharString");
    static const rule<class CIdentifier, std::string> C_identifier("C_Identifier");
    static const rule<class StartBit, uint64_t> start_bit("StartBit");
    static const rule<class SignalSize, uint64_t> signal_size("SignalSize");
    static const rule<class Factor, double> factor("Factor");
    static const rule<class Offset, double> offset("Offset");
    static const rule<class Minimum, double> minimum("Minimum");
    static const rule<class Maximum, double> maximum("Maximum");
    static const rule<class ByteOrder, char> byte_order("ByteOrder");
    static const rule<class ValueType, char> value_type("ValueType");
    static const rule<class Version, std::string> version("Version");
    static const rule<class NewSymbols, std::vector<std::string>> new_symbols("NewSymbols");
    static const rule<class NewSymbol, std::string> new_symbol("NewSymbol");
    static const rule<class BitTiming, G_BitTiming> bit_timing("BitTiming");
    static const rule<class Baudrate, uint64_t> baudrate("baudrate");
    static const rule<class BTR1, uint64_t> btr1("BTR1");
    static const rule<class BTR2, uint64_t> btr2("BTR2");
    static const rule<class Node, G_Node> node("Node");
    static const rule<class Node, std::vector<G_Node>> nodes("Nodes");
    static const rule<class NodeName, std::string> node_name("NodeName");
    static const rule<class ValueTable, G_ValueTable> value_table("ValueTable");
    static const rule<class ValueTableName, std::string> value_table_name("ValueTableName");
    static const rule<class ValueEncodingDescription, G_ValueEncodingDescription> value_encoding_description("ValueEncodingDescription");
    static const rule<class Message, G_Message> message("Message");
    static const rule<class MessageId, uint64_t> message_id("MessageID");
    static const rule<class MessageName, std::string> message_name("MessageName");
    static const rule<class MessageSize, uint64_t> message_size("MessageSize");
    static const rule<class Transmitter, std::string> transmitter("Transmitter");
    static const rule<class Signal, G_Signal> signal("Signal");
    static const rule<class SignalName, std::string> signal_name("SignalName");
    static const rule<class MultiplexerIndicator, boost::optional<std::string>> multiplexer_indicator("MultiplexerIndicator");
    static const rule<class Unit, std::string> unit("Unit");
    static const rule<class Receiver, std::string> receiver("Receiver");
    static const rule<class MessageTransmitter, G_MessageTransmitter> message_transmitter("MessageTransmitter");
    static const rule<class EnvironmentVariable, G_EnvironmentVariable> environment_variable("EnvironmentVariable");
    static const rule<class EnvVarName, std::string> env_var_name("EnvVarName");
    static const rule<class EnvVarType, uint64_t> env_var_type("EnvVarType");
    static const rule<class InitialValue, double> initial_value("InitialValue");
    static const rule<class EvId, uint64_t> ev_id("EvId");
    static const rule<class AccessType, std::string> access_type("AccessType");
    static const rule<class AccessNodes, std::vector<std::string>> access_nodes("AccessNodes");
    static const rule<class EnvironmentVariableData, G_EnvironmentVariableData> environment_variable_data("EnvironmentVariableData");
    static const rule<class DataSize, uint64_t> data_size("DataSize");
    static const rule<class SignalType, G_SignalType> signal_type("SignalType");
    static const rule<class SignalTypeName, std::string> signal_type_name("SignalTypeName");
    static const rule<class DefaultValue, double> default_value("DefaultValue");
    static const rule<class Comment, G_Comment> comment("Comment");
    static const rule<class CommentNetwork, G_CommentNetwork> comment_network("CommentNetwork");
    static const rule<class CommentNode, G_CommentNode> comment_node("CommentNode");
    static const rule<class CommentMessage, G_CommentMessage> comment_message("CommentMessage");
    static const rule<class CommentSignal, G_CommentSignal> comment_signal("CommentSignal");
    static const rule<class CommentEnvVar, G_CommentEnvVar> comment_env_var("CommentEnvVar");
    static const rule<class AttributeDefinition, G_AttributeDefinition> attribute_definition("AttributeDefinition");
    static const rule<class ObjectType, boost::optional<std::string>> object_type("ObjectType");
    static const rule<class AttributeName, std::string> attribute_name("AttributeName");
    static const rule<class AttributeValueType, G_AttributeValue> attribute_value_type("AttributeValueType");
    static const rule<class AttributeValueTypeInt, G_AttributeValueTypeInt> attribute_value_type_int("AttributeValueTypeInt");
    static const rule<class AttributeValueTypeHex, G_AttributeValueTypeHex> attribute_value_type_hex("AttributeValueTypeHex");
    static const rule<class AttributeValueTypeFloat, G_AttributeValueTypeFloat> attribute_value_type_float("AttributeValueTypeFloat");
    static const rule<class AttributeValueTypeString, G_AttributeValueTypeString> attribute_value_type_string("AttributeValueTypeString");
    static const rule<class AttributeValueTypeEnum, G_AttributeValueTypeEnum> attribute_value_type_enum("AttributeValueTypeEnum");
    static const rule<class AttributeDefault, G_Attribute> attribute_default("AttributeDefault");
    static const rule<class AttributeValue, variant_attr_value_t> attribute_value("AttributeValue");
    static const rule<class AttributeValueEnt, variant_attribute_t> attribute_value_ent("AttributeValueEnt");
    static const rule<class AttributeValueEntNetwork, G_AttributeNetwork> attribute_value_ent_network("AttributeValueEntNetwork");
    static const rule<class AttributeValueEntNode, G_AttributeNode> attribute_value_ent_node("AttributeValueEntNode");
    static const rule<class AttributeValueEntMessage, G_AttributeMessage> attribute_value_ent_message("AttributeValueEntMessage");
    static const rule<class AttributeValueEntSignal, G_AttributeSignal> attribute_value_ent_signal("AttributeValueEntSignal");
    static const rule<class AttributeValueEntEnvVar, G_AttributeEnvVar> attribute_value_ent_env_var("AttributeValueEntEnvVar");
    static const rule<class ValueDescriptionSigEnvVar, G_ValueDescriptionSigEnvVar> value_description_sig_env_var("ValueDescription");
    static const rule<class ValueDescriptionSignal, G_ValueDescriptionSignal> value_description_signal("ValueDescriptionSignal");
    static const rule<class ValueDescriptionEnvVar, G_ValueDescriptionEnvVar> value_description_env_var("ValueDescriptionEnvVar");
    static const rule<class SignalExtendedValueType, G_SignalExtendedValueType> signal_extended_value_type("SignalExtendedValueType");
        
    static const rule<class Network, G_Network> network("Network");

    static const auto network_def =
          version
        > new_symbols
        > bit_timing
        > nodes
        > *value_table
        > *message
        > *message_transmitter
        > *environment_variable
        > *environment_variable_data
        > *signal_type
        > *comment
        > *attribute_definition
        > *attribute_default
        > *attribute_value_ent

        > *value_description_sig_env_var
        > *signal_extended_value_type
        ;

    static const auto unsigned_int = uint64;
    static const auto signed_int= int64;

    static const auto single_line_comment = "//" >> *(char_ - eol) >> (eol | eoi);
    static const auto block_comment_def   = ("/*" >> *(block_comment | char_ - "*/")) > "*/";
    static const auto skipper             = space | single_line_comment | block_comment;

    static const auto quoted_string_def = lexeme['"' >> *(('\\' >> char_("\\\"")) | ~char_('"')) >> '"'];
    static const auto C_identifier_def = lexeme[char_("a-zA-Z_") >> *char_("a-zA-Z_0-9")];
    static const auto start_bit_def = unsigned_int;
    static const auto signal_size_def = unsigned_int;
    static const auto factor_def = double_;
    static const auto offset_def = double_;
    static const auto minimum_def = double_;
    static const auto maximum_def = double_;
    static const auto byte_order_def = lexeme[char_('0') | char_('1')];
    static const auto value_type_def = lexeme[char_('-') | char_('+')];
    static const auto version_def = lit("VERSION") > quoted_string;
    
    static auto SetVal = [](auto& ctx) { _val(ctx) = _attr(ctx); };
    static const auto new_symbol_def =
        lexeme[
              string("SIGTYPE_VALTYPE_")[SetVal]
            | string("BA_DEF_DEF_REL_")[SetVal]
            | string("BA_DEF_SGTYPE_")[SetVal]
            | string("SIG_TYPE_REF_")[SetVal]
            | string("ENVVAR_DATA_")[SetVal]
            | string("SIG_VALTYPE_")[SetVal]
            | string("SG_MUL_VAL_")[SetVal]
            | string("BA_DEF_DEF_")[SetVal]
            | string("ENVVAR_DTA_")[SetVal]
            | string("BA_DEF_REL_")[SetVal]
            | string("SGTYPE_VAL_")[SetVal]
            | string("VAL_TABLE_")[SetVal]
            | string("BA_SGTYPE_")[SetVal]
            | string("SIG_GROUP_")[SetVal]
            | string("BU_SG_REL_")[SetVal]
            | string("BU_EV_REL_")[SetVal]
            | string("BU_BO_REL_")[SetVal]
            | string("BO_TX_BU_")[SetVal]
            | string("NS_DESC_")[SetVal]
            | string("CAT_DEF_")[SetVal]
            | string("EV_DATA_")[SetVal]
            | string("BA_REL_")[SetVal]
            | string("SGTYPE_")[SetVal]
            | string("BA_DEF_")[SetVal]
            | string("FILTER")[SetVal]
            | string("DEF_")[SetVal]
            | string("VAL_")[SetVal]
            | string("CAT_")[SetVal]
            | string("BA_")[SetVal]
            | string("CM_")[SetVal]
        ];
    static const auto new_symbols_def = lit("NS_") >> ':' > *new_symbol;

    static const auto bit_timing_def = lit("BS_") >> ':' >> -(baudrate >> ':' >> btr1 >> ',' >> btr2);
    static const auto baudrate_def = unsigned_int;
    static const auto btr1_def = unsigned_int;
    static const auto btr2_def = unsigned_int;

    static const auto nodes_def = lit("BU_") >> ':' >> skip(ascii::blank)[*node];
    static const auto node_def = node_name;
    static const auto node_name_def = C_identifier;

    static const auto value_table_def = lit("VAL_TABLE_") > value_table_name > *value_encoding_description > ';';
    static const auto value_table_name_def = C_identifier;
    static const auto value_encoding_description_def = signed_int > quoted_string;

    static const auto message_def = lexeme[lit("BO_") >> omit[space]] > message_id > message_name
        > ':' > message_size > skip(blank)[transmitter > eol] > *signal;
    static const auto message_id_def = unsigned_int;
    static const auto message_name_def = C_identifier;
    static const auto message_size_def = unsigned_int;
    static const auto transmitter_def = node_name;

    static const auto signal_def = lexeme[lit("SG_") >> omit[space]] > signal_name
        > multiplexer_indicator > ':' > start_bit > '|' > signal_size > '@' > byte_order > value_type
        > '(' > factor > ',' > offset > ')' > '[' > minimum > '|' > maximum > ']' > unit >> skip(blank)[*receiver > eol];
    static const auto signal_name_def = C_identifier;
    static const auto multiplexer_indicator_def = -C_identifier;
    static const auto unit_def = quoted_string;
    static const auto receiver_def = node_name;

    static const auto message_transmitter_def = lexeme[lit("BO_TX_BU_") >> omit[space]] > message_id > ':' > (transmitter % ',') > ';';
    
    static const auto environment_variable_def =
            lexeme[lit("EV_") >> omit[space]] > env_var_name > ':' > env_var_type > '[' > minimum > '|' > maximum
        > ']' > unit > initial_value > ev_id > access_type > access_nodes > ';';
    static const auto env_var_name_def = C_identifier;
    static const auto env_var_type_def = unsigned_int;
    static const auto initial_value_def = double_;
    static const auto ev_id_def = unsigned_int;
    static const auto access_type_def = C_identifier;
    static const auto access_nodes_def = node_name % ',';
    
    static const auto environment_variable_data_def = lexeme[lit("ENVVAR_DATA_") >> omit[space]] > env_var_name > ':' > data_size > ';';
    static const auto data_size_def = unsigned_int;
    
    static const auto signal_type_def =
          lexeme[lit("SGTYPE_") >> omit[space]] > signal_type_name > ':' > signal_size > '@' > byte_order
        > value_type > '(' > factor > ',' > offset > ')' > '[' > minimum > '|' > maximum > ']' > unit
        >> default_value > ',' > value_table_name > ';';
    static const auto signal_type_name_def = C_identifier;
    static const auto default_value_def = double_;
    
    static const auto comment_def =
            lexeme[lit("CM_")
        >> omit[space]] > (comment_node | comment_message | comment_signal | comment_env_var | comment_network);
    static const auto comment_network_def = quoted_string > ';';
    static const auto comment_node_def = lit("BU_") > node_name > quoted_string > ';';
    static const auto comment_message_def = lit("BO_") > message_id > quoted_string > ';';
    static const auto comment_signal_def = lit("SG_") > message_id > signal_name > quoted_string > ';';
    static const auto comment_env_var_def = lit("EV_") > env_var_name > quoted_string > ';';
    
    static const auto attribute_definition_def =
        lexeme[lit("BA_DEF_") >> omit[space]] > object_type > attribute_name > attribute_value_type > ';';
    
    static const auto object_type_def =
        -(string("BU_")[SetVal]
        | string("BO_")[SetVal]
        | string("SG_")[SetVal]
        | string("EV_")[SetVal]);
    static const auto attribute_name_def = quoted_string;
    static const auto attribute_value_type_def =
          attribute_value_type_int | attribute_value_type_hex | attribute_value_type_float
        | attribute_value_type_string | attribute_value_type_enum;
    static const auto attribute_value_type_int_def = lit("INT") > signed_int > signed_int;
    static const auto attribute_value_type_hex_def = lit("HEX") > signed_int > signed_int;
    static const auto attribute_value_type_float_def = lit("FLOAT") > double_ > double_;
    static auto SetAttributeValueTypeString = [](auto& ctx) { _val(ctx) = G_AttributeValueTypeString(); };
    static const auto attribute_value_type_string_def = lit("STRING")[SetAttributeValueTypeString];
    static auto SetAttributeValuetypeEnum = [](auto& ctx) { _val(ctx).values = _attr(ctx); };
    static const auto attribute_value_type_enum_def = (lit("ENUM") > (quoted_string % ','))[SetAttributeValuetypeEnum];
    
    static const auto attribute_default_def =
        (lexeme[(lit("BA_DEF_DEF_REL_") | lit("BA_DEF_DEF_")) >> omit[space]]) > attribute_name > attribute_value > ';';
    static const auto attribute_value_def = double_ | signed_int | quoted_string;
    
    static const auto attribute_value_ent_def = 
            lexeme[lit("BA_") >> omit[space]]
        > (attribute_value_ent_network | attribute_value_ent_node
            | attribute_value_ent_message | attribute_value_ent_signal
            | attribute_value_ent_env_var) > ';';
    static const auto attribute_value_ent_network_def = attribute_name >> attribute_value;
    static const auto attribute_value_ent_node_def = attribute_name >> lit("BU_") > node_name > attribute_value;
    static const auto attribute_value_ent_message_def = attribute_name >> lit("BO_") > message_id > attribute_value;
    static const auto attribute_value_ent_signal_def = attribute_name >> lit("SG_") > message_id > signal_name > attribute_value;
    static const auto attribute_value_ent_env_var_def = attribute_name >> lit("EV_") > env_var_name > attribute_value;
    
    static const auto value_description_sig_env_var_def = value_description_signal | value_description_env_var;
    static const auto value_description_signal_def =
        lexeme[lit("VAL_") >> omit[space]] >> message_id > signal_name > *value_encoding_description > ';';
    static const auto value_description_env_var_def =
        lexeme[lit("VAL_") >> omit[space]] >> env_var_name > *value_encoding_description > ';';

    static const auto signal_extended_value_type_def =
        lexeme[lit("SIG_VALTYPE_") > omit[space]] > message_id > signal_name > ':' > unsigned_int > ';';
    
    BOOST_SPIRIT_DEFINE(block_comment)
    BOOST_SPIRIT_DEFINE(quoted_string)
    BOOST_SPIRIT_DEFINE(C_identifier)
    BOOST_SPIRIT_DEFINE(start_bit) 
    BOOST_SPIRIT_DEFINE(signal_size)
    BOOST_SPIRIT_DEFINE(factor)
    BOOST_SPIRIT_DEFINE(offset)
    BOOST_SPIRIT_DEFINE(minimum)
    BOOST_SPIRIT_DEFINE(maximum)
    BOOST_SPIRIT_DEFINE(byte_order)
    BOOST_SPIRIT_DEFINE(value_type)
    BOOST_SPIRIT_DEFINE(version)
    BOOST_SPIRIT_DEFINE(new_symbol)
    BOOST_SPIRIT_DEFINE(new_symbols)
    BOOST_SPIRIT_DEFINE(bit_timing)
    BOOST_SPIRIT_DEFINE(baudrate)
    BOOST_SPIRIT_DEFINE(btr1)
    BOOST_SPIRIT_DEFINE(btr2)
    BOOST_SPIRIT_DEFINE(node)
    BOOST_SPIRIT_DEFINE(nodes)
    BOOST_SPIRIT_DEFINE(node_name)
    BOOST_SPIRIT_DEFINE(value_table)
    BOOST_SPIRIT_DEFINE(value_table_name)
    BOOST_SPIRIT_DEFINE(value_encoding_description)
    BOOST_SPIRIT_DEFINE(message)
    BOOST_SPIRIT_DEFINE(message_id)
    BOOST_SPIRIT_DEFINE(message_name)
    BOOST_SPIRIT_DEFINE(message_size)
    BOOST_SPIRIT_DEFINE(transmitter)
    BOOST_SPIRIT_DEFINE(signal)
    BOOST_SPIRIT_DEFINE(signal_name)
    BOOST_SPIRIT_DEFINE(multiplexer_indicator)
    BOOST_SPIRIT_DEFINE(unit)
    BOOST_SPIRIT_DEFINE(receiver)
    BOOST_SPIRIT_DEFINE(message_transmitter)
    BOOST_SPIRIT_DEFINE(environment_variable)
    BOOST_SPIRIT_DEFINE(env_var_name)
    BOOST_SPIRIT_DEFINE(env_var_type)
    BOOST_SPIRIT_DEFINE(initial_value)
    BOOST_SPIRIT_DEFINE(ev_id)
    BOOST_SPIRIT_DEFINE(access_type)
    BOOST_SPIRIT_DEFINE(access_nodes)
    BOOST_SPIRIT_DEFINE(environment_variable_data)
    BOOST_SPIRIT_DEFINE(data_size)
    BOOST_SPIRIT_DEFINE(signal_type)
    BOOST_SPIRIT_DEFINE(signal_type_name)
    BOOST_SPIRIT_DEFINE(default_value)
    BOOST_SPIRIT_DEFINE(comment)
    BOOST_SPIRIT_DEFINE(comment_network)
    BOOST_SPIRIT_DEFINE(comment_node)
    BOOST_SPIRIT_DEFINE(comment_message)
    BOOST_SPIRIT_DEFINE(comment_signal)
    BOOST_SPIRIT_DEFINE(comment_env_var)
    BOOST_SPIRIT_DEFINE(attribute_definition)
    BOOST_SPIRIT_DEFINE(object_type)
    BOOST_SPIRIT_DEFINE(attribute_name)
    BOOST_SPIRIT_DEFINE(attribute_value_type)
    BOOST_SPIRIT_DEFINE(attribute_value_type_int)
    BOOST_SPIRIT_DEFINE(attribute_value_type_hex)
    BOOST_SPIRIT_DEFINE(attribute_value_type_float)
    BOOST_SPIRIT_DEFINE(attribute_value_type_string)
    BOOST_SPIRIT_DEFINE(attribute_value_type_enum)
    BOOST_SPIRIT_DEFINE(attribute_default)
    BOOST_SPIRIT_DEFINE(attribute_value)
    BOOST_SPIRIT_DEFINE(attribute_value_ent)
    BOOST_SPIRIT_DEFINE(attribute_value_ent_network)
    BOOST_SPIRIT_DEFINE(attribute_value_ent_node)
    BOOST_SPIRIT_DEFINE(attribute_value_ent_message)
    BOOST_SPIRIT_DEFINE(attribute_value_ent_signal)
    BOOST_SPIRIT_DEFINE(attribute_value_ent_env_var)
    BOOST_SPIRIT_DEFINE(value_description_signal)
    BOOST_SPIRIT_DEFINE(value_description_env_var)
    BOOST_SPIRIT_DEFINE(value_description_sig_env_var)
    BOOST_SPIRIT_DEFINE(signal_extended_value_type)
    BOOST_SPIRIT_DEFINE(network)
}
std::optional<dbcppp::DBCX3::AST::G_Network> dbcppp::DBCX3::ParseFromMemory(const char* begin, const char* end)
{
    dbcppp::DBCX3::AST::G_Network gnet;
    if (phrase_parse(begin, end, Grammar::network, Grammar::skipper, gnet) && begin == end)
    {
        return gnet;
    }
    if (begin != end)
    {
        throw std::runtime_error("Could not parse DBC (begin != end)");
    }
        throw std::runtime_error("Could not parse DBC");
}