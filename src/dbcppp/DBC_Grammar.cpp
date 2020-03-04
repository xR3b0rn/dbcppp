
#include <istream>
#include <cstdint>
#include <charconv>
#include <ostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string_view>
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

#include "DBC_Grammar.h"
#include "NetworkImpl.h"
#include "ConvertGrammarStructureToCppStructure.h"

using namespace dbcppp;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace sp = boost::spirit::labels;
namespace ph = boost::phoenix;
namespace fu = boost::fusion;

BOOST_FUSION_ADAPT_STRUCT(
	G_Version,
	position,
	version
)
BOOST_FUSION_ADAPT_STRUCT(
	G_NewSymbols,
	position,
	new_symbols
)
BOOST_FUSION_ADAPT_STRUCT(
	G_BitTiming,
	position,
	baudrate, BTR1, BTR2
)
BOOST_FUSION_ADAPT_STRUCT(
	G_Node,
	position,
	name
)
BOOST_FUSION_ADAPT_STRUCT(
	G_ValueTable,
	position,
	name, value_encoding_descriptions
)
BOOST_FUSION_ADAPT_STRUCT(
	G_Signal,
	position,
	name, multiplexer_indicator,
	start_bit, signal_size,
	byte_order, value_type,
	factor, offset, minimum,
	maximum, unit, receivers
)
BOOST_FUSION_ADAPT_STRUCT(
	G_Message,
	position,
	id, name, size,
	transmitter, signals
)
BOOST_FUSION_ADAPT_STRUCT(
	G_MessageTransmitter,
	position,
	id, transmitters
)
BOOST_FUSION_ADAPT_STRUCT(
	G_EnvironmentVariable,
	position,
	name, var_type, minimum,
	maximum, unit, initial_value,
	id, access_type, access_nodes
)
BOOST_FUSION_ADAPT_STRUCT(
	G_EnvironmentVariableData,
	position,
	name, size
)
BOOST_FUSION_ADAPT_STRUCT(
	G_SignalType,
	position,
	name, size, byte_order, value_type,
	factor, offset, minimum, maximum,
	unit, default_value, value_table_name
)
BOOST_FUSION_ADAPT_STRUCT(
	G_CommentNetwork,
	position,
	comment
)
BOOST_FUSION_ADAPT_STRUCT(
	G_CommentNode,
	position,
	node_name, comment
)
BOOST_FUSION_ADAPT_STRUCT(
	G_CommentMessage,
	position,
	message_id, comment
)
BOOST_FUSION_ADAPT_STRUCT(
	G_CommentSignal,
	position,
	message_id, signal_name, comment
)
BOOST_FUSION_ADAPT_STRUCT(
	G_CommentEnvVar,
	position,
	env_var_name, comment
)
BOOST_FUSION_ADAPT_STRUCT(
	G_Comment,
	position,
	comment
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeValueTypeInt,
	position,
	minimum, maximum
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeValueTypeHex,
	position,
	minimum, maximum
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeValueTypeFloat,
	position,
	minimum, maximum
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeValueTypeString,
	position
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeValueTypeEnum,
	position,
	values
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeValue,
	position,
	value
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeDefinition,
	position,
	object_type, name, value_type
)
BOOST_FUSION_ADAPT_STRUCT(
	G_Attribute,
	position,
	name, value
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeNetwork,
	position,
	attribute_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeNode,
	position,
	attribute_name, node_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeMessage,
	position,
	attribute_name, message_id, value
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeSignal,
	position,
	attribute_name, message_id,
	signal_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeEnvVar,
	position,
	attribute_name, env_var_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
	G_ValueDescriptionSignal,
	position,
	message_id, signal_name, value_descriptions
)
BOOST_FUSION_ADAPT_STRUCT(
	G_ValueDescriptionEnvVar,
	position,
	env_var_name, value_descriptions
)
BOOST_FUSION_ADAPT_STRUCT(
	G_ValueDescription,
	position,
	description
)
BOOST_FUSION_ADAPT_STRUCT(
	G_SignalExtendedValueType,
	position,
	message_id, signal_name, value
)
BOOST_FUSION_ADAPT_STRUCT(
	G_Network,
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
	: public qi::grammar<Iter, G_Network(), ascii::space_type>
{
public:
	using Skipper = ascii::space_type;

	NetworkGrammar(Iter begin)
		: NetworkGrammar::base_type(_network, "DBC_Network")
	{
		using boost::spirit::repository::qi::iter_pos;

		_network.name("Network");
		_network %=
			   iter_pos
			>> _version
			>> _new_symbols
			>> _bit_timing
			>> _nodes
			>> _value_tables
			>> _messages
			>> _message_transmitters
			>> _environment_variables
			>> _environment_variable_datas
			>> _signal_types
			>> _comments
			>> _attribute_definitions
			>> _attribute_defaults
			>> _attribute_values

			>> _value_descriptions
			>> _signal_extended_value_types
			;

		_unsigned_integer.name("uint");
		_unsigned_integer %= qi::uint_;
		_signed_integer.name("int");
		_signed_integer %= qi::int_;
		_double.name("double");
		_double %= qi::double_;
		_char_string.name("QuotedString");
		_char_string %= qi::lexeme['"' >> *(('\\' >> qi::char_("\\\"")) | ~qi::char_('"')) >> '"'];
		_C_identifier.name("Identifier");
		_C_identifier %= qi::lexeme[qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9")];
		_C_identifier_.name("Identifier");
		_C_identifier_ %= qi::lexeme[qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9")];

		_version.name("Version");
		_version %= iter_pos >> qi::lit("VERSION") > _char_string;
		
		_new_symbol.name("NewSymbol");
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

		_new_symbols.name("NS_");
		_new_symbols %= qi::lit("NS_") > ':' > *_new_symbol;

		_bit_timing.name("BitTiming");
		_bit_timing %= qi::lit("BS_") >> ':' >> -_bit_timing_inner;
		_bit_timing_inner %= iter_pos >> _baudrate >> ':' >> _BTR1 >> ',' >> _BTR2;
		_baudrate.name("Baudrate");
		_baudrate %= _unsigned_integer;
		_BTR1.name("BTR1");
		_BTR1 %= _unsigned_integer;
		_BTR2.name("BTR2");
		_BTR2 %= _unsigned_integer;
		
		_nodes.name("Nodes");
		_nodes %= qi::lit("BU_") > ':' > qi::skip(ascii::blank)[*_node_ > qi::eol];
		_node_.name("Node");
		_node_ %= iter_pos >> _node_name_;
		_node_name.name("NodeName");
		_node_name %= _C_identifier;
		_node_name.name("NodeName");
		_node_name_ %= _C_identifier_;

		_value_tables.name("ValueTables");
		_value_tables %= *_value_table;
		_value_table.name("ValueTable");
		_value_table %= iter_pos >> qi::lit("VAL_TABLE_") > _value_table_name > _value_encoding_descriptions > ';';
		_value_table_name.name("ValueTableName");
		_value_table_name %= _C_identifier;
		_value_encoding_descriptions.name("VAlueEncodingDescriptions");
		_value_encoding_descriptions %= *_value_encoding_description;
		_value_encoding_description.name("VAlueEncodingDescription");
		_value_encoding_description %= _double > _char_string;

		_messages.name("Messages");
		_messages %= *_message;
		_message.name("Message");
		_message %= iter_pos >> qi::lit("BO_ ") > _message_id > _message_name > ':' > _message_size > _transmitter > _signals;
		_message_id.name("MessageId");
		_message_id %= _unsigned_integer;
		_message_name.name("MessageName");
		_message_name %= _C_identifier;
		_message_size.name("MessageSize");
		_message_size %= _unsigned_integer;
		_transmitter.name("NodeName");
		_transmitter %= _node_name;

		_signals.name("Signals");
		_signals %= *_signal;
		_signal.name("Signal");
		_signal %= iter_pos >> qi::lexeme[qi::lit("SG_") >> qi::omit[qi::space]] > _signal_name > _multiplexer_indicator > ':' > _start_bit > '|'
			> _signal_size > '@' > _byte_order > _value_type > '('
			> _factor > ',' > _offset > ')' > '[' > _minimum > '|' > _maximum > ']' > _unit > _receivers;
		_signal_name.name("SignalName");
		_signal_name %= _C_identifier;
		_multiplexer_indicator.name("SignalMultiplexerIndicator");
		_multiplexer_indicator %= -_C_identifier;
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
		_maximum %= _double;
		_unit.name("Unit");
		_unit %= _char_string;
		_receivers.name("NodeNames");
		_receivers %= qi::skip(ascii::blank)[*_receiver_ > qi::eol];
		_receiver_.name("NodeName");
		_receiver_ %= _node_name_;

		_message_transmitters.name("MessageTransmitters");
		_message_transmitters %= *_message_transmitter;
		_message_transmitter.name("MessageTransmitter");
		_message_transmitter %= iter_pos >> qi::lexeme[qi::lit("BO_TX_BU_") >> qi::omit[qi::space]] > _message_id > ':' > _transmitters > ';';
		_transmitters.name("NodeNames");
		_transmitters %= _transmitter % ',';

		_environment_variables.name("EnvironmentVariables");
		_environment_variables %= *_environment_variable;
		_environment_variable.name("EnvironmentVariable");
		_environment_variable %= iter_pos >> qi::lexeme[qi::lit("EV_") >> qi::omit[qi::space]] > _env_var_name > ':' > _env_var_type
			> '[' > _minimum > '|' > _maximum > ']' > _unit > _initial_value > _ev_id > _access_type > _access_nodes > ';';
		_env_var_name.name("EnvVarName");
		_env_var_name %= _C_identifier;
		_env_var_type.name("EnvVarType");
		_env_var_type %= _unsigned_integer;
		_initial_value.name("InitialValue");
		_initial_value %= _double;
		_ev_id.name("EvId");
		_ev_id %= _unsigned_integer;
		_access_type.name("AccessType");
		_access_type %= _C_identifier;
		_access_nodes.name("NodeNames");
		_access_nodes %= _node_name % ',';

		_environment_variable_datas.name("EnvironmentVariableDatas");
		_environment_variable_datas %= *_environment_variable_data;
		_environment_variable_data.name("EnvironmentVariableData");
		_environment_variable_data %= iter_pos >> qi::lexeme[qi::lit("ENVVAR_DATA_") >> qi::omit[qi::space]] > _env_var_name > ':' > _data_size > ';';
		_data_size.name("DataSize");
		_data_size %= _unsigned_integer;

		_signal_types.name("SignalTypes");
		_signal_types %= *_signal_type;
		_signal_type.name("SignalType");
		_signal_type %= iter_pos >> qi::lexeme[qi::lit("SGTYPE_") >> qi::omit[qi::space]] > _signal_type_name > ':' > _signal_size > '@'
			> _byte_order > _value_type > '(' > _factor > ',' > _offset > ')' > '['
			> _minimum > '|' >> _maximum > ']' > _unit > _default_value > ',' > _value_table_name > ';';
		_signal_type_name.name("SignalTypeName");
		_signal_type_name %= _C_identifier;
		_default_value.name("DefaultValue");
		_default_value %= _double;
		
		_comments.name("Comments");
		_comments %= *_comment;
		_comment.name("Comment");
		_comment %= qi::lexeme[qi::lit("CM_") >> qi::omit[qi::space]]
			> (_comment_node | _comment_message | _comment_signal | _comment_env_var | _comment_network);
		_comment_network.name("CommentNetwork");
		_comment_network %= iter_pos >> _char_string > ';';
		_comment_node.name("CommentNode");
		_comment_node %= iter_pos >> qi::lit("BU_") > _node_name > _char_string > ';';
		_comment_message.name("CommentMessage");
		_comment_message %= iter_pos >> qi::lit("BO_") > _message_id > _char_string > ';';
		_comment_signal.name("CommentSignal");
		_comment_signal %= iter_pos >> qi::lit("SG_") > _message_id > _signal_name > _char_string > ';';
		_comment_env_var.name("CommentEnvVar");
		_comment_env_var %= iter_pos >> qi::lit("EV_") > _env_var_name > _char_string > ';';

		_attribute_definitions.name("AttributeDefinitions");
		_attribute_definitions %= *_attribute_definition;
		_attribute_definition.name("AttributeDefinition");
		_attribute_definition %= iter_pos >> qi::lexeme[qi::lit("BA_DEF_") >> qi::omit[qi::space]] > _object_type > _attribute_name
			> _attribute_value_type > ';';
		_object_type.name("ObjectType");
		_object_type %= -(qi::string("BU_") | qi::string("BO_") | qi::string("SG_") | qi::string("EV_"));
		_attribute_name.name("AttributeName");
		_attribute_name %= _char_string;
		_attribute_value_type.name("AttributeValueType");
		_attribute_value_type %= iter_pos >>
			  (_attribute_value_type_int | _attribute_value_type_hex
			| _attribute_value_type_float | _attribute_value_type_string
			| _attribute_value_type_enum);
		_attribute_value_type_int.name("AttributeValueTypeInt");
		_attribute_value_type_int %= iter_pos >> qi::lit("INT") > _signed_integer > _signed_integer;
		_attribute_value_type_hex.name("AttributeValueTypeHex");
		_attribute_value_type_hex %= iter_pos >> qi::lit("HEX") > _signed_integer > _signed_integer;
		_attribute_value_type_float.name("AttributeValueTypeFloat");
		_attribute_value_type_float %= iter_pos >> qi::lit("FLOAT") > _double > _double;
		_attribute_value_type_string.name("AttributeValueTypeString");
		_attribute_value_type_string %= iter_pos >> qi::lit("STRING");
		_attribute_value_type_enum.name("AttributeValueTypeEnum");
		_attribute_value_type_enum %= iter_pos >> qi::lit("ENUM") > (_char_string % ',');
		
		_attribute_defaults.name("AttributeDefaults");
		_attribute_defaults %= *_attribute_default;
		_attribute_default.name("AttributeDefault");
		_attribute_default %= iter_pos >> (qi::lexeme[(qi::lit("BA_DEF_DEF_REL_") | qi::lit("BA_DEF_DEF_"))  >> qi::omit[qi::space]])
			> _attribute_name > _attribute_value > ';';
		_attribute_value.name("AttributeValue");
		_attribute_value %= _double | _signed_integer | _char_string;

		_attribute_values.name("AttributeValues");
		_attribute_values %= *_attribute_value_ent;
		_attribute_value_ent.name("AttributeValueEnt");
		_attribute_value_ent %= qi::lexeme[qi::lit("BA_") >> qi::omit[qi::space]]
			>> (_attribute_value_ent_network | _attribute_value_ent_node
			  | _attribute_value_ent_message | _attribute_value_ent_signal
			  | _attribute_value_ent_env_var) > ';';
		_attribute_value_ent_network.name("AttributeValueEntNetwork");
		_attribute_value_ent_network %= iter_pos >> _attribute_name >> _attribute_value;
		_attribute_value_ent_node.name("AttributeValueEntNode");
		_attribute_value_ent_node %= iter_pos >> _attribute_name >> qi::lit("BU_") >> _node_name >> _attribute_value;
		_attribute_value_ent_message.name("AttributeValueEntMessage");
		_attribute_value_ent_message %= iter_pos >> _attribute_name >> qi::lit("BO_") >> _message_id >> _attribute_value;
		_attribute_value_ent_signal.name("AttributeValueEntSignal");
		_attribute_value_ent_signal %= iter_pos >> _attribute_name >> qi::lit("SG_") >> _message_id >> _signal_name >> _attribute_value;
		_attribute_value_ent_env_var.name("AttributeValueEntEnvVar");
		_attribute_value_ent_env_var %= iter_pos >> _attribute_name >> qi::lit("EV_") >> _env_var_name >> _attribute_value;

		_value_descriptions.name("ValueDescriptions");
		_value_descriptions %= *_value_description_sig_env_var;
		_value_description_sig_env_var.name("ValueDescriptionsSigEnvVar");
		_value_description_sig_env_var %= iter_pos >> (_value_description_signal | _value_description_env_var);
		_value_description_signal.name("ValueDescriptionsSignal");
		_value_description_signal %= iter_pos >> qi::lexeme[qi::lit("VAL_") >> qi::omit[qi::space]] >> _message_id >> _signal_name >> _value_encoding_descriptions > ';';
		_value_description_env_var.name("ValueDescriptionsEnvVar");
		_value_description_env_var %= iter_pos >> qi::lexeme[qi::lit("VAL_") >> qi::omit[qi::space]] >> _env_var_name >> _value_encoding_descriptions > ';';

		_signal_extended_value_types.name("SignalExtendedValueTypes");
		_signal_extended_value_types %= *_signal_extended_value_type;
		_signal_extended_value_type.name("SignalExtendedValueType");
		_signal_extended_value_type %= iter_pos >> qi::lexeme[qi::lit("SIG_VALTYPE_") >> qi::omit[qi::space]] > _message_id > _signal_name > ':' >  _unsigned_integer > ';';
		
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
	qi::rule<Iter, G_Network(), Skipper> _network;
	
	qi::rule<Iter, uint64_t(), Skipper> _unsigned_integer;
	qi::rule<Iter, int64_t(), Skipper> _signed_integer;
	qi::rule<Iter, double(), Skipper> _double;
	qi::rule<Iter, std::string(), Skipper> _char_string;
	qi::rule<Iter, std::string(), Skipper> _C_identifier;
	qi::rule<Iter, std::string(), ascii::blank_type> _C_identifier_;
	
	qi::rule<Iter, G_Version, Skipper> _version;
	qi::rule<Iter, std::string(), Skipper> _new_symbol;
	qi::rule<Iter, std::vector<std::string>(), Skipper> _new_symbols;
	qi::rule<Iter, boost::optional<G_BitTiming>, Skipper> _bit_timing;
	qi::rule<Iter, G_BitTiming, Skipper> _bit_timing_inner;
	qi::rule<Iter, uint64_t(), Skipper> _baudrate;
	qi::rule<Iter, uint64_t(), Skipper> _BTR1;
	qi::rule<Iter, uint64_t(), Skipper> _BTR2;

	qi::rule<Iter, std::vector<G_Node>(), Skipper> _nodes;
	qi::rule<Iter, std::string(), Skipper> _node_name;
	qi::rule<Iter, G_Node(), ascii::blank_type> _node_;
	qi::rule<Iter, std::vector<std::string>(), ascii::blank_type> _node_names;
	qi::rule<Iter, std::string(), ascii::blank_type> _node_name_;

	qi::rule<Iter, std::vector<G_ValueTable>(), Skipper> _value_tables;
	qi::rule<Iter, G_ValueTable(), Skipper> _value_table;
	qi::rule<Iter, std::string(), Skipper> _value_table_name;
	qi::rule<Iter, std::map<double, std::string>(), Skipper> _value_encoding_descriptions;
	qi::rule<Iter, std::pair<double, std::string>(), Skipper> _value_encoding_description;

	qi::rule<Iter, std::vector<G_Message>(), Skipper> _messages;
	qi::rule<Iter, G_Message(), Skipper> _message;
	qi::rule<Iter, uint64_t(), Skipper> _message_id;
	qi::rule<Iter, std::string(), Skipper> _message_name;
	qi::rule<Iter, uint64_t(), Skipper> _message_size;
	qi::rule<Iter, std::string(), Skipper> _transmitter;
	
	qi::rule<Iter, std::vector<G_Signal>(), Skipper> _signals;
	qi::rule<Iter, G_Signal(), Skipper> _signal;
	qi::rule<Iter, std::string(), Skipper> _signal_name;
	qi::rule<Iter, boost::optional<std::string>(), Skipper> _multiplexer_indicator;
	qi::rule<Iter, uint64_t(), Skipper> _start_bit;
	qi::rule<Iter, uint64_t(), Skipper> _signal_size;
	qi::rule<Iter, char(), Skipper> _byte_order;
	qi::rule<Iter, char(), Skipper> _value_type;
	qi::rule<Iter, double(), Skipper> _factor;
	qi::rule<Iter, double(), Skipper> _offset;
	qi::rule<Iter, double(), Skipper> _maximum;
	qi::rule<Iter, double(), Skipper> _minimum;
	qi::rule<Iter, std::string(), Skipper> _unit;
	qi::rule<Iter, std::vector<std::string>(), Skipper> _receivers;
	qi::rule<Iter, std::string(), ascii::blank_type> _receiver_;

	qi::rule<Iter, std::vector<G_MessageTransmitter>(), Skipper> _message_transmitters;
	qi::rule<Iter, G_MessageTransmitter(), Skipper> _message_transmitter;
	qi::rule<Iter, std::vector<std::string>(), Skipper> _transmitters;

	qi::rule<Iter, std::vector<G_EnvironmentVariable>(), Skipper> _environment_variables;
	qi::rule<Iter, G_EnvironmentVariable(), Skipper> _environment_variable;
	qi::rule<Iter, std::string(), Skipper> _env_var_name;
	qi::rule<Iter, uint64_t(), Skipper> _env_var_type;
	qi::rule<Iter, double(), Skipper> _initial_value;
	qi::rule<Iter, uint64_t(), Skipper> _ev_id;
	qi::rule<Iter, std::string(), Skipper> _access_type;
	qi::rule<Iter, std::vector<std::string>(), Skipper> _access_nodes;
	
	qi::rule<Iter, std::vector<G_EnvironmentVariableData>(), Skipper> _environment_variable_datas;
	qi::rule<Iter, G_EnvironmentVariableData(), Skipper> _environment_variable_data;
	qi::rule<Iter, uint64_t(), Skipper> _data_size;
	
	qi::rule<Iter, std::vector<G_SignalType>(), Skipper> _signal_types;
	qi::rule<Iter, G_SignalType(), Skipper> _signal_type;
	qi::rule<Iter, std::string(), Skipper> _signal_type_name;
	qi::rule<Iter, double(), Skipper> _default_value;

	qi::rule<Iter, std::vector<variant_comment_t>(), Skipper> _comments;
	qi::rule<Iter, variant_comment_t(), Skipper> _comment;
	qi::rule<Iter, G_CommentNetwork(), Skipper> _comment_network;
	qi::rule<Iter, G_CommentNode(), Skipper> _comment_node;
	qi::rule<Iter, G_CommentMessage(), Skipper> _comment_message;
	qi::rule<Iter, G_CommentSignal(), Skipper> _comment_signal;
	qi::rule<Iter, G_CommentEnvVar(), Skipper> _comment_env_var;

	qi::rule<Iter, std::vector<G_AttributeDefinition>(), Skipper> _attribute_definitions;
	qi::rule<Iter, G_AttributeDefinition(), Skipper> _attribute_definition;
	qi::rule<Iter, boost::optional<std::string>(), Skipper> _object_type;
	qi::rule<Iter, std::string(), Skipper> _attribute_name;
	qi::rule<Iter, G_AttributeValue(), Skipper> _attribute_value_type;
	qi::rule<Iter, G_AttributeValueTypeInt(), Skipper> _attribute_value_type_int;
	qi::rule<Iter, G_AttributeValueTypeHex(), Skipper> _attribute_value_type_hex;
	qi::rule<Iter, G_AttributeValueTypeFloat(), Skipper> _attribute_value_type_float;
	qi::rule<Iter, G_AttributeValueTypeString(), Skipper> _attribute_value_type_string;
	qi::rule<Iter, G_AttributeValueTypeEnum(), Skipper> _attribute_value_type_enum;

	qi::rule<Iter, std::vector<G_Attribute>(), Skipper> _attribute_defaults;
	qi::rule<Iter, G_Attribute(), Skipper> _attribute_default;
	qi::rule<Iter, variant_attr_value_t(), Skipper> _attribute_value;

	qi::rule<Iter, std::vector<variant_attribute_t>(), Skipper> _attribute_values;
	qi::rule<Iter, variant_attribute_t(), Skipper> _attribute_value_ent;
	qi::rule<Iter, G_AttributeNetwork(), Skipper> _attribute_value_ent_network;
	qi::rule<Iter, G_AttributeNode(), Skipper> _attribute_value_ent_node;
	qi::rule<Iter, G_AttributeMessage(), Skipper> _attribute_value_ent_message;
	qi::rule<Iter, G_AttributeMessage(), Skipper> _attribute_value_ent_message2;
	qi::rule<Iter, G_AttributeSignal(), Skipper> _attribute_value_ent_signal;
	qi::rule<Iter, G_AttributeEnvVar(), Skipper> _attribute_value_ent_env_var;

	qi::rule<Iter, std::vector<G_ValueDescription>(), Skipper> _value_descriptions;
	qi::rule<Iter, G_ValueDescription(), Skipper> _value_description_sig_env_var;
	qi::rule<Iter, G_ValueDescriptionSignal(), Skipper> _value_description_signal;
	qi::rule<Iter, G_ValueDescriptionEnvVar(), Skipper> _value_description_env_var;

	qi::rule<Iter, std::vector<G_SignalExtendedValueType>(), Skipper> _signal_extended_value_types;
	qi::rule<Iter, G_SignalExtendedValueType(), Skipper> _signal_extended_value_type;
};
std::unique_ptr<Network> Network::fromDBC(std::istream& is)
{
	std::unique_ptr<Network> result;
	std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
	auto begin{str.begin()}, end{str.end()};
	NetworkGrammar<std::string::iterator> g(begin);
	G_Network gnet;
	bool succeeded = phrase_parse(begin, end, g, ascii::space, gnet);
	succeeded &= begin == end;
	if (!succeeded)
	{
		auto[line, column] = getErrPos(str.begin(), begin);
		std::cout << line << ":" << column << " Error! Unexpected token near here!" << std::endl;
	}
	if (succeeded)
	{
		auto network = ConvertGrammarStructureToCppStructure(gnet);
		result = std::make_unique<NetworkImpl>(std::move(network));
	}
	return result;
}
std::unique_ptr<Network> Network::fromDBC(std::istream& is, std::unique_ptr<Network> network)
{
	auto other = fromDBC(is);
	network->merge(std::move(other));
	return std::move(network);
}
extern "C"
{
	DBCPPP_API struct dbcppp_Network* dbcppp_load_from_file(const char* filename)
	{
		std::ifstream is(filename);
		if (!is.is_open())
		{
			return nullptr;
		}
		auto unique_ptr_network = Network::fromDBC(is);
		auto ptr_network = unique_ptr_network.get();
		unique_ptr_network.release();
		return reinterpret_cast<dbcppp_Network*>(const_cast<Network*>(ptr_network));
	}
	DBCPPP_API void free_network(struct dbcppp_network* network)
	{
		delete reinterpret_cast<NetworkImpl*>(network);
	}
}
