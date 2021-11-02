
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/fusion/include/io.hpp>

#include "NetworkImpl.h"
#include "DBCX3.h"

#include "DBCX3AdaptStructs.inl"

namespace dbcppp::DBCX3::Grammar
{
    using namespace dbcppp::DBCX3::AST;
    using namespace boost::spirit::x3;
    
    struct error_handler
    {
        template <typename Iterator, typename Exception, typename Context>
        error_handler_result on_error(
              Iterator& first, const Iterator& last
            , const Exception& x, const Context& context)
        {
            auto& error_handler = get<error_handler_tag>(context).get();
            std::string message = "Error! Expecting: " + x.which() + " here:";
            error_handler(x.where(), message);
            return error_handler_result::fail;
        }
    };

    static const rule<struct TagBlockComment> block_comment("BlockComment");
    static const rule<struct TagCharString, std::string> quoted_string("CharString");
    static const rule<struct TagCIdentifier, std::string> C_identifier("C_Identifier");
    static const rule<struct TagStartBit, uint64_t> start_bit("StartBit");
    static const rule<struct TagSignalSize, uint64_t> signal_size("SignalSize");
    static const rule<struct TagFactor, double> factor("Factor");
    static const rule<struct TagOffset, double> offset("Offset");
    static const rule<struct TagMinimum, double> minimum("Minimum");
    static const rule<struct TagMaximum, double> maximum("Maximum");
    static const rule<struct TagByteOrder, char> byte_order("ByteOrder");
    static const rule<struct TagValueType, char> value_type("ValueType");
    static const rule<struct TagVersion, std::string> version("Version");
    static const rule<struct TagNewSymbols, std::vector<std::string>> new_symbols("NewSymbols");
    static const rule<struct TagNewSymbol, std::string> new_symbol("NewSymbol");
    static const rule<struct TagBitTiming, G_BitTiming> bit_timing("BitTiming");
    static const rule<struct TagBaudrate, uint64_t> baudrate("baudrate");
    static const rule<struct TagBTR1, uint64_t> btr1("BTR1");
    static const rule<struct TagBTR2, uint64_t> btr2("BTR2");
    static const rule<struct TagNode, G_Node> node("Node");
    static const rule<struct TagNode, std::vector<G_Node>> nodes("Nodes");
    static const rule<struct TagNodeName, std::string> node_name("NodeName");
    static const rule<struct TagValueTable, G_ValueTable> value_table("ValueTable");
    static const rule<struct TagValueTableName, std::string> value_table_name("ValueTableName");
    static const rule<struct TagValueEncodingDescription, G_ValueEncodingDescription> value_encoding_description("ValueEncodingDescription");
    static const rule<struct TagMessage, G_Message> message("Message");
    static const rule<struct TagMessageId, uint64_t> message_id("MessageID");
    static const rule<struct TagMessageName, std::string> message_name("MessageName");
    static const rule<struct TagMessageSize, uint64_t> message_size("MessageSize");
    static const rule<struct TagTransmitter, std::string> transmitter("Transmitter");
    static const rule<struct TagSignal, G_Signal> signal("Signal");
    static const rule<struct TagSignalName, std::string> signal_name("SignalName");
    static const rule<struct TagMultiplexerIndicator, boost::optional<std::string>> multiplexer_indicator("MultiplexerIndicator");
    static const rule<struct TagUnit, std::string> unit("Unit");
    static const rule<struct TagReceiver, std::string> receiver("Receiver");
    static const rule<struct TagMessageTransmitter, G_MessageTransmitter> message_transmitter("MessageTransmitter");
    static const rule<struct TagEnvironmentVariable, G_EnvironmentVariable> environment_variable("EnvironmentVariable");
    static const rule<struct TagEnvVarName, std::string> env_var_name("EnvVarName");
    static const rule<struct TagEnvVarType, uint64_t> env_var_type("EnvVarType");
    static const rule<struct TagInitialValue, double> initial_value("InitialValue");
    static const rule<struct TagEvId, uint64_t> ev_id("EvId");
    static const rule<struct TagAccessType, std::string> access_type("AccessType");
    static const rule<struct TagAccessNodes, std::vector<std::string>> access_nodes("AccessNodes");
    static const rule<struct TagEnvironmentVariableData, G_EnvironmentVariableData> environment_variable_data("EnvironmentVariableData");
    static const rule<struct TagDataSize, uint64_t> data_size("DataSize");
    static const rule<struct TagSignalType, G_SignalType> signal_type("SignalType");
    static const rule<struct TagSignalTypeName, std::string> signal_type_name("SignalTypeName");
    static const rule<struct TagDefaultValue, double> default_value("DefaultValue");
    static const rule<struct TagComment, G_Comment> comment("Comment");
    static const rule<struct TagCommentNetwork, G_CommentNetwork> comment_network("CommentNetwork");
    static const rule<struct TagCommentNode, G_CommentNode> comment_node("CommentNode");
    static const rule<struct TagCommentMessage, G_CommentMessage> comment_message("CommentMessage");
    static const rule<struct TagCommentSignal, G_CommentSignal> comment_signal("CommentSignal");
    static const rule<struct TagCommentEnvVar, G_CommentEnvVar> comment_env_var("CommentEnvVar");
    static const rule<struct TagAttributeDefinition, G_AttributeDefinition> attribute_definition("AttributeDefinition");
    static const rule<struct TagObjectType, boost::optional<std::string>> object_type("ObjectType");
    static const rule<struct TagAttributeName, std::string> attribute_name("AttributeName");
    static const rule<struct TagAttributeValueType, G_AttributeValue> attribute_value_type("AttributeValueType");
    static const rule<struct TagAttributeValueTypeInt, G_AttributeValueTypeInt> attribute_value_type_int("AttributeValueTypeInt");
    static const rule<struct TagAttributeValueTypeHex, G_AttributeValueTypeHex> attribute_value_type_hex("AttributeValueTypeHex");
    static const rule<struct TagAttributeValueTypeFloat, G_AttributeValueTypeFloat> attribute_value_type_float("AttributeValueTypeFloat");
    static const rule<struct TagAttributeValueTypeString, G_AttributeValueTypeString> attribute_value_type_string("AttributeValueTypeString");
    static const rule<struct TagAttributeValueTypeEnum, G_AttributeValueTypeEnum> attribute_value_type_enum("AttributeValueTypeEnum");
    static const rule<struct TagAttributeDefault, G_Attribute> attribute_default("AttributeDefault");
    static const rule<struct TagAttributeValue, variant_attr_value_t> attribute_value("AttributeValue");
    static const rule<struct TagAttributeValueEnt, variant_attribute_t> attribute_value_ent("AttributeValueEnt");
    static const rule<struct TagAttributeValueEntNetwork, G_AttributeNetwork> attribute_value_ent_network("AttributeValueEntNetwork");
    static const rule<struct TagAttributeValueEntNode, G_AttributeNode> attribute_value_ent_node("AttributeValueEntNode");
    static const rule<struct TagAttributeValueEntMessage, G_AttributeMessage> attribute_value_ent_message("AttributeValueEntMessage");
    static const rule<struct TagAttributeValueEntSignal, G_AttributeSignal> attribute_value_ent_signal("AttributeValueEntSignal");
    static const rule<struct TagAttributeValueEntEnvVar, G_AttributeEnvVar> attribute_value_ent_env_var("AttributeValueEntEnvVar");
    static const rule<struct TagValueDescriptionSigEnvVar, G_ValueDescriptionSigEnvVar> value_description_sig_env_var("ValueDescription");
    static const rule<struct TagValueDescriptionSignal, G_ValueDescriptionSignal> value_description_signal("ValueDescriptionSignal");
    static const rule<struct TagValueDescriptionEnvVar, G_ValueDescriptionEnvVar> value_description_env_var("ValueDescriptionEnvVar");
    static const rule<struct TagSignalExtendedValueType, G_SignalExtendedValueType> signal_extended_value_type("SignalExtendedValueType");
    static const rule<struct TagRange, G_Range> range("Range");
    static const rule<struct TagSignalMultiplexerValue, G_SignalMultiplexerValue> signal_multiplexer_value("SignalMultiplexerValue");
    static const rule<struct TagSignalSignalGroup, G_SignalGroup> signal_group("SignalGroup");
    
    static const rule<struct TagNetwork, G_Network> network("Network");

    static const auto network_def =
          version
        > -new_symbols
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
        > *signal_group
        > *signal_extended_value_type
        > *signal_multiplexer_value
        > eoi
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

    static const auto value_table_def = lexeme[lit("VAL_TABLE_") >> omit[skipper]] > value_table_name > *value_encoding_description > ';';
    static const auto value_table_name_def = C_identifier;
    static const auto value_encoding_description_def = signed_int > quoted_string;

    static const auto message_def = lexeme[lit("BO_") >> omit[skipper]] > message_id > message_name
        > ':' > message_size > skip(blank)[transmitter > (eol | eoi)] > *signal;
    static const auto message_id_def = unsigned_int;
    static const auto message_name_def = C_identifier;
    static const auto message_size_def = unsigned_int;
    static const auto transmitter_def = node_name;

    static const auto signal_def = lexeme[lit("SG_") >> omit[space]] > signal_name
        > multiplexer_indicator > ':' > start_bit > '|' > signal_size > '@' > byte_order > value_type
        > '(' > factor > ',' > offset > ')' > '[' > minimum > '|' > maximum > ']' > unit >> skip(blank)[(receiver % ',') > (eol | eoi)];
    static const auto signal_name_def = C_identifier;
    static const auto multiplexer_indicator_def = -C_identifier;
    static const auto unit_def = quoted_string;
    static const auto receiver_def = node_name;

    static const auto message_transmitter_def = lexeme[lit("BO_TX_BU_") >> omit[space]] > message_id > ':' > (transmitter % ',') > ';';
    
    static const auto environment_variable_def =
            lexeme[lit("EV_") >> omit[skipper]] > env_var_name > ':' > env_var_type > '[' > minimum > '|' > maximum
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
          lexeme[lit("SGTYPE_") >> omit[skipper]] > signal_type_name > ':' > signal_size > '@' > byte_order
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
        lexeme[lit("BA_DEF_") >> omit[skipper]] > object_type > attribute_name > attribute_value_type > ';';
    
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
            lexeme[lit("BA_") >> omit[skipper]]
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
        lexeme[lit("VAL_") >> omit[skipper]] >> message_id > signal_name > *value_encoding_description > ';';
    static const auto value_description_env_var_def =
        lexeme[lit("VAL_") >> omit[skipper]] >> env_var_name > *value_encoding_description > ';';

    static const auto signal_extended_value_type_def =
        lexeme[lit("SIG_VALTYPE_") > omit[skipper]] > message_id > signal_name > ':' > unsigned_int > ';';
    
    static const auto range_def = unsigned_int > '-' > unsigned_int;
    static const auto signal_multiplexer_value_def =
        lexeme[lit("SG_MUL_VAL_") >> omit[skipper]] > unsigned_int > C_identifier > C_identifier > (range % ',') > ';';

    static const auto signal_group_def =
        lexeme[lit("SIG_GROUP_") >> omit[skipper]] > unsigned_int > C_identifier > unsigned_int > ":" > *C_identifier > ';';

    BOOST_SPIRIT_DEFINE(block_comment);
    BOOST_SPIRIT_DEFINE(quoted_string);
    BOOST_SPIRIT_DEFINE(C_identifier);
    BOOST_SPIRIT_DEFINE(start_bit);
    BOOST_SPIRIT_DEFINE(signal_size);
    BOOST_SPIRIT_DEFINE(factor);
    BOOST_SPIRIT_DEFINE(offset);
    BOOST_SPIRIT_DEFINE(minimum);
    BOOST_SPIRIT_DEFINE(maximum);
    BOOST_SPIRIT_DEFINE(byte_order);
    BOOST_SPIRIT_DEFINE(value_type);
    BOOST_SPIRIT_DEFINE(version);
    BOOST_SPIRIT_DEFINE(new_symbol);
    BOOST_SPIRIT_DEFINE(new_symbols);
    BOOST_SPIRIT_DEFINE(bit_timing);
    BOOST_SPIRIT_DEFINE(baudrate);
    BOOST_SPIRIT_DEFINE(btr1);
    BOOST_SPIRIT_DEFINE(btr2);
    BOOST_SPIRIT_DEFINE(node);
    BOOST_SPIRIT_DEFINE(nodes);
    BOOST_SPIRIT_DEFINE(node_name);
    BOOST_SPIRIT_DEFINE(value_table);
    BOOST_SPIRIT_DEFINE(value_table_name);
    BOOST_SPIRIT_DEFINE(value_encoding_description);
    BOOST_SPIRIT_DEFINE(message);
    BOOST_SPIRIT_DEFINE(message_id);
    BOOST_SPIRIT_DEFINE(message_name);
    BOOST_SPIRIT_DEFINE(message_size);
    BOOST_SPIRIT_DEFINE(transmitter);
    BOOST_SPIRIT_DEFINE(signal);
    BOOST_SPIRIT_DEFINE(signal_name);
    BOOST_SPIRIT_DEFINE(multiplexer_indicator);
    BOOST_SPIRIT_DEFINE(unit);
    BOOST_SPIRIT_DEFINE(receiver);
    BOOST_SPIRIT_DEFINE(message_transmitter);
    BOOST_SPIRIT_DEFINE(environment_variable);
    BOOST_SPIRIT_DEFINE(env_var_name);
    BOOST_SPIRIT_DEFINE(env_var_type);
    BOOST_SPIRIT_DEFINE(initial_value);
    BOOST_SPIRIT_DEFINE(ev_id);
    BOOST_SPIRIT_DEFINE(access_type);
    BOOST_SPIRIT_DEFINE(access_nodes);
    BOOST_SPIRIT_DEFINE(environment_variable_data);
    BOOST_SPIRIT_DEFINE(data_size);
    BOOST_SPIRIT_DEFINE(signal_type);
    BOOST_SPIRIT_DEFINE(signal_type_name);
    BOOST_SPIRIT_DEFINE(default_value);
    BOOST_SPIRIT_DEFINE(comment);
    BOOST_SPIRIT_DEFINE(comment_network);
    BOOST_SPIRIT_DEFINE(comment_node);
    BOOST_SPIRIT_DEFINE(comment_message);
    BOOST_SPIRIT_DEFINE(comment_signal);
    BOOST_SPIRIT_DEFINE(comment_env_var);
    BOOST_SPIRIT_DEFINE(attribute_definition);
    BOOST_SPIRIT_DEFINE(object_type);
    BOOST_SPIRIT_DEFINE(attribute_name);
    BOOST_SPIRIT_DEFINE(attribute_value_type);
    BOOST_SPIRIT_DEFINE(attribute_value_type_int);
    BOOST_SPIRIT_DEFINE(attribute_value_type_hex);
    BOOST_SPIRIT_DEFINE(attribute_value_type_float);
    BOOST_SPIRIT_DEFINE(attribute_value_type_string);
    BOOST_SPIRIT_DEFINE(attribute_value_type_enum);
    BOOST_SPIRIT_DEFINE(attribute_default);
    BOOST_SPIRIT_DEFINE(attribute_value);
    BOOST_SPIRIT_DEFINE(attribute_value_ent);
    BOOST_SPIRIT_DEFINE(attribute_value_ent_network);
    BOOST_SPIRIT_DEFINE(attribute_value_ent_node);
    BOOST_SPIRIT_DEFINE(attribute_value_ent_message);
    BOOST_SPIRIT_DEFINE(attribute_value_ent_signal);
    BOOST_SPIRIT_DEFINE(attribute_value_ent_env_var);
    BOOST_SPIRIT_DEFINE(value_description_signal);
    BOOST_SPIRIT_DEFINE(value_description_env_var);
    BOOST_SPIRIT_DEFINE(value_description_sig_env_var);
    BOOST_SPIRIT_DEFINE(signal_extended_value_type);
    BOOST_SPIRIT_DEFINE(range);
    BOOST_SPIRIT_DEFINE(signal_multiplexer_value);
    BOOST_SPIRIT_DEFINE(signal_group);
    BOOST_SPIRIT_DEFINE(network);

    struct TagCharString                    : error_handler, annotate_on_success {};
    struct TagCIdentifier                   : error_handler, annotate_on_success {};
    struct TagStartBit                      : error_handler, annotate_on_success {};
    struct TagSignalSize                    : error_handler, annotate_on_success {};
    struct TagFactor                        : error_handler, annotate_on_success {};
    struct TagOffset                        : error_handler, annotate_on_success {};
    struct TagMinimum                       : error_handler, annotate_on_success {};
    struct TagMaximum                       : error_handler, annotate_on_success {};
    struct TagByteOrder                     : error_handler, annotate_on_success {};
    struct TagValueType                     : error_handler, annotate_on_success {};
    struct TagVersion                       : error_handler, annotate_on_success {};
    struct TagNewSymbols                    : error_handler, annotate_on_success {};
    struct TagNewSymbol                     : error_handler, annotate_on_success {};
    struct TagBitTiming                     : error_handler, annotate_on_success {};
    struct TagBaudrate                      : error_handler, annotate_on_success {};
    struct TagBTR1                          : error_handler, annotate_on_success {};
    struct TagBTR2                          : error_handler, annotate_on_success {};
    struct TagNode                          : error_handler, annotate_on_success {};
    struct TagNodeName                      : error_handler, annotate_on_success {};
    struct TagValueTable                    : error_handler, annotate_on_success {};
    struct TagValueTableName                : error_handler, annotate_on_success {};
    struct TagValueEncodingDescription      : error_handler, annotate_on_success {};
    struct TagTagMessage                    : error_handler, annotate_on_success {};
    struct TagMessageId                     : error_handler, annotate_on_success {};
    struct TagMessageName                   : error_handler, annotate_on_success {};
    struct TagMessageSize                   : error_handler, annotate_on_success {};
    struct TagTransmitter                   : error_handler, annotate_on_success {};
    struct TagSignal                        : error_handler, annotate_on_success {};
    struct TagSignalName                    : error_handler, annotate_on_success {};
    struct TagMultiplexerIndicator          : error_handler, annotate_on_success {};
    struct TagUnit                          : error_handler, annotate_on_success {};
    struct TagReceiver                      : error_handler, annotate_on_success {};
    struct TagMessageTransmitter            : error_handler, annotate_on_success {};
    struct TagEnvironmentVariable           : error_handler, annotate_on_success {};
    struct TagEnvVarName                    : error_handler, annotate_on_success {};
    struct TagEnvVarType                    : error_handler, annotate_on_success {};
    struct TagInitialValue                  : error_handler, annotate_on_success {};
    struct TagEvId                          : error_handler, annotate_on_success {};
    struct TagAccessType                    : error_handler, annotate_on_success {};
    struct TagAccessNodes                   : error_handler, annotate_on_success {};
    struct TagEnvironmentVariableData       : error_handler, annotate_on_success {};
    struct TagDataSize                      : error_handler, annotate_on_success {};
    struct TagSignalType                    : error_handler, annotate_on_success {};
    struct TagSignalTypeName                : error_handler, annotate_on_success {};
    struct TagDefaultValue                  : error_handler, annotate_on_success {};
    struct TagComment                       : error_handler, annotate_on_success {};
    struct TagCommentNetwork                : error_handler, annotate_on_success {};
    struct TagCommentNode                   : error_handler, annotate_on_success {};
    struct TagCommentMessage                : error_handler, annotate_on_success {};
    struct TagCommentSignal                 : error_handler, annotate_on_success {};
    struct TagCommentEnvVar                 : error_handler, annotate_on_success {};
    struct TagAttributeDefinition           : error_handler, annotate_on_success {};
    struct TagObjectType                    : error_handler, annotate_on_success {};
    struct TagAttributeName                 : error_handler, annotate_on_success {};
    struct TagAttributeValueType            : error_handler, annotate_on_success {};
    struct TagAttributeValueTypeInt         : error_handler, annotate_on_success {};
    struct TagAttributeValueTypeHex         : error_handler, annotate_on_success {};
    struct TagAttributeValueTypeFloat       : error_handler, annotate_on_success {};
    struct TagAttributeValueTypeString      : error_handler, annotate_on_success {};
    struct TagAttributeValueTypeEnum        : error_handler, annotate_on_success {};
    struct TagAttributeDefault              : error_handler, annotate_on_success {};
    struct TagAttributeValue                : error_handler, annotate_on_success {};
    struct TagAttributeValueEnt             : error_handler, annotate_on_success {};
    struct TagAttributeValueEntNetwork      : error_handler, annotate_on_success {};
    struct TagAttributeValueEntNode         : error_handler, annotate_on_success {};
    struct TagAttributeValueEntMessage      : error_handler, annotate_on_success {};
    struct TagAttributeValueEntSignal       : error_handler, annotate_on_success {};
    struct TagAttributeValueEntEnvVar       : error_handler, annotate_on_success {};
    struct TagValueDescriptionSigEnvVar     : error_handler, annotate_on_success {};
    struct TagValueDescriptionSignal        : error_handler, annotate_on_success {};
    struct TagValueDescriptionEnvVar        : error_handler, annotate_on_success {};
    struct TagSignalExtendedValueType       : error_handler, annotate_on_success {};
    struct TagRange                         : error_handler, annotate_on_success {};
    struct TagSignalMultiplexerValue        : error_handler, annotate_on_success {};
    struct TagNetwork                       : error_handler, annotate_on_success {};
}
std::optional<dbcppp::DBCX3::AST::G_Network> dbcppp::DBCX3::ParseFromMemory(const char* begin, const char* end)
{
    using boost::spirit::x3::with;
    using boost::spirit::x3::error_handler_tag;
    using error_handler_type = boost::spirit::x3::error_handler<const char*>;
    error_handler_type error_handler(begin, end, std::cerr);
    auto const parser =
        with<error_handler_tag>(std::ref(error_handler))
        [
            dbcppp::DBCX3::Grammar::network
        ];
    dbcppp::DBCX3::AST::G_Network gnet;
    if (phrase_parse(begin, end, parser, Grammar::skipper, gnet) && begin == end)
    {
        return gnet;
    }
    return std::nullopt;
}