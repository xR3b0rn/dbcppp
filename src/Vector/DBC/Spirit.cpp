/*
    This file is part of the dbcppp (DBC C++ parser) program.
    Copyright (C) 2019 Julian Hindelang

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <cstdint>
#include <ostream>
#include <fstream>
#include <iterator>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/phoenix.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/qi_hold.hpp>

#include "Signal.h"
#include <Vector/DBC/Message.h>
#include <Vector/DBC/Network.h>
#include <Vector/DBC/BitTiming.h>
#include <Vector/DBC/Node.h>
#include <Vector/DBC/EnvironmentVariable.h>
#include <Vector/DBC/AttributeDefinition.h>
#include <Vector/DBC/Attribute.h>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace sp = boost::spirit::labels;
namespace ph = boost::phoenix;
namespace fu = boost::fusion;

using namespace Vector::DBC;

BOOST_FUSION_ADAPT_STRUCT(
	Signal,
	name, startBit,
	bitSize, byteOrder, valueType,
	factor, offset, minimum,
	maximum, unit, receivers,
	comment, type, multiplexor,
	multiplexerSwitchValue, extendedValueType
)
BOOST_FUSION_ADAPT_STRUCT(
	Message,
	id, name, size,
	transmitter, signals
)
BOOST_FUSION_ADAPT_STRUCT(
	Network
)
BOOST_FUSION_ADAPT_STRUCT(
	BitTiming,
	baudrate, btr1, btr2
)
BOOST_FUSION_ADAPT_STRUCT(
	Node, name
)
BOOST_FUSION_ADAPT_STRUCT(
	ValueTable,
	name, valueDescriptions, valueDescriptions
)
BOOST_FUSION_ADAPT_STRUCT(
	EnvironmentVariable,
	name, type, minimum, maximum,
	unit, initialValue, id,
	accessType, accessNodes
)
BOOST_FUSION_ADAPT_STRUCT(
	AttributeValueType
)
BOOST_FUSION_ADAPT_STRUCT(
	AttributeDefinition,
	objectType, name, valueType
)
BOOST_FUSION_ADAPT_STRUCT(
	Attribute,
	name
)

namespace boost { namespace spirit { namespace traits
{
	template <>
	struct transform_attribute<std::pair<std::string, Signal>, Signal, boost::spirit::qi::domain>
	{
		using type = Signal&;
		using pair_t = std::pair<std::string, Signal>;
		static Signal& pre(pair_t& pair)
		{
			return pair.second;
		}
		static void post(pair_t& pair, const Signal& sig)
		{
			pair.first = sig.name;
			pair.second = sig;
		}
		static void fail(pair_t& pair)
		{
		}
	};
	template <>
	struct transform_attribute<std::pair<uint32_t, Message>, Message, boost::spirit::qi::domain>
	{
		using type = Message&;
		using pair_t = std::pair<uint32_t, Message>;
		static Message& pre(pair_t& pair)
		{
			return pair.second;
		}
		static void post(pair_t& pair, const Message& msg)
		{
			pair.first = msg.id;
			pair.second = msg;
		}
		static void fail(pair_t& pair)
		{
		}
	};
	template <>
	struct transform_attribute<std::pair<std::string, Node>, Node, boost::spirit::qi::domain>
	{
		using type = Node&;
		using pair_t = std::pair<std::string, Node>;
		static Node& pre(pair_t& pair)
		{
			return pair.second;
		}
		static void post(pair_t& pair, const Node& n)
		{
			pair.first = n.name;
			pair.second = n;
		}
		static void fail(pair_t& pair)
		{
		}
	};
	template <>
	struct transform_attribute<std::pair<std::string, ValueTable>, ValueTable, boost::spirit::qi::domain>
	{
		using type = ValueTable&;
		using pair_t = std::pair<std::string, ValueTable>;
		static ValueTable& pre(pair_t& pair)
		{
			return pair.second;
		}
		static void post(pair_t& pair, const ValueTable& n)
		{
			pair.first = n.name;
			pair.second = n;
		}
		static void fail(pair_t& pair)
		{
		}
	};
}}}
struct SymByteOrder
	: boost::spirit::qi::symbols<char, ByteOrder>
{
	SymByteOrder()
	{
		add
			("0", ByteOrder::BigEndian)
			("1", ByteOrder::LittleEndian);
	}
} g_sym_byte_order;
struct SymValueType
	: boost::spirit::qi::symbols<char, ValueType>
{
	SymValueType()
	{
		add
			("+", ValueType::Unsigned)
			("-", ValueType::Signed);
	}
} g_sym_value_type;
struct SymMultiplexor
	: boost::spirit::qi::symbols<char, Signal::Multiplexor>
{
	SymMultiplexor()
	{
		add
			("0", Signal::Multiplexor::NoMultiplexor)
			("m", Signal::Multiplexor::MultiplexedSignal);
			("M", Signal::Multiplexor::MultiplexorSwitch);
	}
} g_sym_multiplexor;
struct SymEnvVarType
	: boost::spirit::qi::symbols<char, EnvironmentVariable::Type>
{
	SymEnvVarType()
	{
		add
			("0", EnvironmentVariable::Type::Integer)
			("1", EnvironmentVariable::Type::Float)
			("s", EnvironmentVariable::Type::String)
			("d", EnvironmentVariable::Type::Data);
	}
} g_sym_env_var_type;
struct SymEnvVarAccessType
	: boost::spirit::qi::symbols<char, EnvironmentVariable::AccessType>
{
	SymEnvVarAccessType()
	{
		add
			("0", EnvironmentVariable::AccessType::Unrestricted)
			("1", EnvironmentVariable::AccessType::Read)
			("2", EnvironmentVariable::AccessType::Write)
			("3", EnvironmentVariable::AccessType::ReadWrite);
	}
} g_sym_env_var_access_type;
struct SymAttributeObjectType
	: boost::spirit::qi::symbols<char, AttributeObjectType>
{
	SymAttributeObjectType()
	{
		add
			("", AttributeObjectType::Network)
			("BU_", AttributeObjectType::Node)
			("BO_", AttributeObjectType::Message)
			("SG_", AttributeObjectType::Signal)
			("EV_", AttributeObjectType::EnvironmentVariable)
			("BU_EV_REL_", AttributeObjectType::ControlUnitEnvironmentVariable)
			("BU_BO_REL_", AttributeObjectType::NodeTxMessage)
			("BU_SG_REL_", AttributeObjectType::NodeMappedRxSignal);
	}
} g_sym_attribute_object_type;

template <class T> void s_set_attribute_value(int32_t value, T context) { std::get<0>(context.attributes.car).integerValue = value; };
template <class T> void s_set_attribute_value(double value, T context) { std::get<0>(context.attributes.car).floatValue = value; };
template <class T> void s_set_attribute_value(const std::string& value, T context) { std::get<0>(context.attributes.car).stringValue = value; };

auto set_attribute_network_value =
	[](const auto& value, auto context)
	{
		std::get<0>(context.attributes.car).objectType = AttributeObjectType::Network;
		boost::apply_visitor([&context](const auto& value) { s_set_attribute_value(value, context); }, value);
	};
auto set_attribute_node_value =
	[](const auto& value, auto context)
	{
		std::get<0>(context.attributes.car).objectType = AttributeObjectType::Node;
		boost::apply_visitor([&context](const auto& value) { s_set_attribute_value(value, context); }, value);
	};
auto set_attribute_message_value =
	[](const auto& value, auto context)
	{
		std::get<0>(context.attributes.car).objectType = AttributeObjectType::Message;
		boost::apply_visitor([&context](const auto& value) { s_set_attribute_value(value, context); }, value);
	};
auto set_attribute_signal_value =
	[](const auto& value, auto context)
	{
		std::get<0>(context.attributes.car).objectType = AttributeObjectType::Signal;
		boost::apply_visitor([&context](const auto& value) { s_set_attribute_value(value, context); }, value);
	};

auto set_attribute_value_type_int =
	[](const auto& attr, auto context)
	{
		context.attributes.car.type = AttributeValueType::Type::Int;
		context.attributes.car.integerValue.minimum = fu::at_c<0>(attr);
		context.attributes.car.integerValue.maximum = fu::at_c<1>(attr);
	};
auto set_attribute_value_type_hex =
	[](const auto& attr, auto context)
	{
		context.attributes.car.type = AttributeValueType::Type::Hex;
		context.attributes.car.hexValue.minimum = fu::at_c<0>(attr);
		context.attributes.car.hexValue.maximum = fu::at_c<1>(attr);
	};
auto set_attribute_value_type_float =
	[](const auto& attr, auto context)
	{
		context.attributes.car.type = AttributeValueType::Type::Float;
		context.attributes.car.floatValue.minimum = fu::at_c<0>(attr);
		context.attributes.car.floatValue.maximum = fu::at_c<1>(attr);
	};
auto set_attribute_value_type_string =
	[](const auto& attr, auto context)
	{
		context.attributes.car.type = AttributeValueType::Type::String;
	};
auto set_attribute_value_type_enum =
	[](const std::string& attr, auto context)
	{
		context.attributes.car.type = AttributeValueType::Type::Enum;
		context.attributes.car.enumValues.push_back(attr);
	};

auto set_attribute_default_value_int =
	[](const auto& value, auto context)
	{
		context.attributes.car.integerValue = value;
	};
auto set_attribute_default_value_double =
	[](const auto& value, auto context)
	{
		context.attributes.car.floatValue = value;
	};
auto set_attribute_default_value_string =
	[](const auto& value, auto context)
	{
		context.attributes.car.stringValue = value;
	};

auto set_signal_multiplexor =
	[](const auto& switch_value, auto context)
	{
		if (switch_value)
		{
			context.attributes.car.multiplexor = Signal::Multiplexor::MultiplexedSignal;
			context.attributes.car.multiplexerSwitchValue = *switch_value;
		}
		else
		{
			context.attributes.car.multiplexor = Signal::Multiplexor::MultiplexorSwitch;
		}
	};

auto insert_node_names =
	[](const std::map<std::string, Node>& nodes, auto context)
	{
		for (auto& p : nodes)
		{
			context.attributes.car.nodes[p.first].name = p.second.name;
		}
	};
auto insert_message_transmitter_by_transmitter =
	[](const std::pair<uint32_t, std::set<std::string>>& transmitter, auto context)
	{
		context.attributes.car.messages[transmitter.first].transmitters = transmitter.second;
	};
auto insert_message_by_message =
	[](const Message& msg, auto context)
	{
		Message& ex = context.attributes.car.messages[msg.id];
		ex.id = msg.id;
		ex.name = msg.name;
		ex.size = msg.size;
		ex.transmitter = msg.transmitter;
		for (auto& p : msg.signals)
		{
			Signal& ex_sig = ex.signals[p.first];
			ex_sig.name = p.second.name;
			ex_sig.multiplexor = p.second.multiplexor;
			ex_sig.multiplexerSwitchValue = p.second.multiplexerSwitchValue;
			ex_sig.startBit = p.second.startBit;
			ex_sig.bitSize = p.second.bitSize;
			ex_sig.byteOrder = p.second.byteOrder;
			ex_sig.valueType = p.second.valueType;
			ex_sig.factor = p.second.factor;
			ex_sig.offset = p.second.offset;
			ex_sig.minimum = p.second.minimum;
			ex_sig.maximum = p.second.maximum;
			ex_sig.unit = p.second.unit;
			ex_sig.receivers = p.second.receivers;
		}
	};
auto insert_value_table_by_value_table =
	[](const ValueTable& table, auto context)
	{
		context.attributes.car.valueTables[table.name] = table;
	};
auto insert_environment_variable_by_environment_variable =
	[](const EnvironmentVariable& var, auto context)
	{
		EnvironmentVariable& ex = context.attributes.car.environmentVariables[var.name];
		ex.name = var.name;
		ex.type = var.type;
		ex.minimum = var.minimum;
		ex.maximum = var.maximum;
		ex.unit = var.unit;
		ex.initialValue = var.initialValue;
		ex.id = var.id;
		ex.accessType = var.accessType;
		ex.accessNodes = var.accessNodes;
	};
auto set_version =
	[](const std::string& version, auto context)
	{
		context.attributes.car.version = version;
	};
auto set_bit_timing =
	[](const BitTiming& timing, auto context)
	{
		context.attributes.car.bitTiming = timing;
	};
auto set_new_symbols =
	[](const std::vector<std::string>& new_syms, auto context)
	{
		context.attributes.car.newSymbols = new_syms;
	};
auto set_network_comment =
	[](const std::string& comment, auto context)
	{
		context.attributes.car.comment = comment;
	};
auto set_node_comment =
	[](const std::tuple<std::string, std::string>& comment, auto context)
	{
		context.attributes.car.nodes[std::get<0>(comment)].comment = std::get<1>(comment);
	};
auto set_message_comment =
	[](const std::tuple<uint32_t, std::string>& comment, auto context)
	{
		context.attributes.car.messages[std::get<0>(comment)].comment = std::get<1>(comment);
	};
auto set_signal_comment =
	[](const std::tuple<uint32_t, std::string, std::string>& comment, auto context)
	{
		context.attributes.car.messages[std::get<0>(comment)].signals[std::get<1>(comment)].comment = std::get<2>(comment);
	};
auto set_env_comment =
	[](const std::tuple<std::string, std::string>& comment, auto context)
	{
		context.attributes.car.environmentVariables[std::get<0>(comment)].comment = std::get<1>(comment);
	};
auto insert_value_descriptions_for_signal_by_value_descprtions =
	[](const std::tuple<uint32_t, std::string, std::map<uint32_t, std::string>>& desc, auto context)
	{
		context.attributes.car.messages[std::get<0>(desc)].signals[std::get<1>(desc)].valueDescriptions = std::get<2>(desc);
	};
auto insert_value_descriptions_for_env_by_value_descprtions =
	[](const std::tuple<std::string, std::map<uint32_t, std::string>>& desc, auto context)
	{
		context.attributes.car.environmentVariables[std::get<0>(desc)].valueDescriptions = std::get<1>(desc);
	};
auto insert_attribute_definitions_by_attribute_definition =
	[](const AttributeDefinition& attr, auto context)
	{
		context.attributes.car.attributeDefinitions.insert(std::make_pair(attr.name, attr));
	};
auto insert_attribute_default_by_attribute_default =
	[](const Attribute& attr, auto context)
	{
		AttributeDefinition& attr_def = context.attributes.car.attributeDefinitions[attr.name];
		Attribute& def = context.attributes.car.attributeDefaults[attr.name];
		def.name = attr.name;
		def.objectType = def.objectType;
		switch (attr_def.valueType.type)
		{
		case AttributeValueType::Type::Int: def.integerValue = attr.integerValue; break;
		case AttributeValueType::Type::Hex: def.hexValue = attr.hexValue; break;
		case AttributeValueType::Type::Float: def.floatValue = attr.floatValue; break;
		case AttributeValueType::Type::String: def.stringValue = attr.stringValue; break;
		case AttributeValueType::Type::Enum: def.stringValue = attr.stringValue; break;
		}
	};

template <class ITER>
struct NetworkGrammar
	: public qi::grammar<ITER, Network(), ascii::space_type>
{
	NetworkGrammar()
		: NetworkGrammar::base_type(start_)
	{
		ident_ %= +(qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9"));
		quoted_string_ %= qi::lexeme['"' >> *(('\\' >> qi::char_("\\\"")) | ~qi::char_('"')) >> '"'];
		version_ %= qi::lit("VERSION") >> quoted_string_;
		new_symbols_ %=
			qi::lexeme
			[
				qi::lit("NS_")
				>> qi::omit[*qi::blank >> ':' >> *qi::blank >> qi::eol]
				>> *(+qi::omit[qi::blank] >> ident_ >> qi::omit[*qi::blank >> qi::eol])
			];
		bit_timing_ %=
			qi::lit("BS_") >> ':'
			>> -(qi::int_ >> ':' >> qi::int_ >> ',' >> qi::int_);

		node_ %= ident_;
		nodes_ %=
			qi::lexeme
			[
				qi::lit("BU_") >> qi::omit[*qi::blank] >> ':'
				>> *(qi::omit[+qi::blank] >> qi::attr_cast(node_)) >> qi::eol
			];

		value_table_ %=
			qi::lit("VAL_TABLE_")
			>> ident_
			>> *(qi::uint_ >> ident_);

		transmitter_ %= qi::lit("VECTOR_XXX") | (ident_ - ';');
		transmitters_ %= *transmitter_;

		message_transmitter_ %=
			qi::lit("BO_TX_BU_")
			>> qi::uint_ >> ':'
			>> transmitters_ >> ';';

		environment_variable_ %=
			qi::lit("EV_")
			>> ident_ >> ':'
			>> g_sym_env_var_type >> '('
			>> qi::double_ >> '|'
			>> qi::double_ >> ')'
			>> ident_
			>> qi::double_
			>> qi::uint_
			>> g_sym_env_var_access_type
			>> ((ident_ - ',') % ',') >> ';';
		
		network_comment_ %= qi::lit("CM_") >> quoted_string_ >> -qi::char_(";");
		node_comment_ %= qi::lit("CM_") >> qi::lit("BU_") >> (ident_ - '"') >> quoted_string_ >> -qi::char_(";");
		message_comment_ %= qi::lit("CM_") >> qi::lit("BO_") >> qi::uint_ >> quoted_string_ >> -qi::char_(";");
		signal_comment_ %= qi::lit("CM_") >> qi::lit("SG_") >> qi::uint_ >> (ident_ - '"') >> quoted_string_ >> -qi::char_(";");
		env_comment_ %= qi::lit("CM_") >> qi::lit("EV_") >> (ident_ - '"') >> quoted_string_ >> -qi::char_(";");

		value_encoding_descriptions_ %= *(qi::uint_ >> quoted_string_);
		value_description_signal_ %=
			qi::lit("VAL_")
			>> qi::uint_
			>> ident_
			>> value_encoding_descriptions_
			>> ';';
		value_description_env_ %=
			qi::lit("VAL_")
			>> ident_
			>> value_encoding_descriptions_
			>> ';';

		attribute_value_ %= qi::int_ | qi::double_ | quoted_string_;
		attribute_value_ba_network_ %=
			qi::lit("BA_") >> quoted_string_
			>> qi::omit[attribute_value_[set_attribute_network_value]]
			>> -qi::lit(";");
		attribute_value_ba_node_ %=
			qi::lit("BA_") >> quoted_string_
			>> qi::lit("BU_") >> ident_
			>> qi::omit[attribute_value_[set_attribute_node_value]]
			>> -qi::lit(";");
		attribute_value_ba_message_ %=
			qi::lit("BA_") >> quoted_string_
			>> qi::lit("BO_") >> qi::uint_
			>> qi::omit[attribute_value_[set_attribute_message_value]]
			>> -qi::lit(";");
		attribute_value_ba_signal_ %=
			qi::lit("BA_") >> quoted_string_
			>> qi::lit("SG_") >> qi::uint_ >> ident_
			>> qi::omit[attribute_value_[set_attribute_signal_value]]
			>> -qi::lit(";");
		attribute_value_ba_env_;

		attribute_value_type_ %=
			qi::omit
			[
				  (qi::lit("INT") >> qi::omit[(qi::int_ >> qi::int_)[set_attribute_value_type_int]])
				| (qi::lit("HEX") >> qi::omit[(qi::int_ >> qi::int_)[set_attribute_value_type_hex]])
				| (qi::lit("FLOAT") >> qi::omit[(qi::double_ >> qi::double_)[set_attribute_value_type_float]])
				| (qi::lit("STRING")[set_attribute_value_type_string])
				| (qi::lit("ENUM") >> qi::omit[quoted_string_[set_attribute_value_type_enum] % ','])
			];
		attribute_definition_ %=
			(qi::lit("BA_DEF_") | qi::lit("BA_DEF_REL_"))
			>> -g_sym_attribute_object_type >> quoted_string_
			>> attribute_value_type_ >> -qi::lit(";");

		attribute_default_ %=
			(qi::lit("BA_DEF_DEF_") | qi::lit("BA_DEF_DEF_REL"))
			>> quoted_string_
			>> qi::omit
			[
				  qi::int_[set_attribute_default_value_int]
				| qi::double_[set_attribute_default_value_double]
				| quoted_string_[set_attribute_default_value_string]
			] >> ';';

		receivers_ %= ident_ % ',';
		signal_ %=
			qi::lit("SG_")
			>> ident_
			>> -qi::omit[('M' | ('m' >> qi::uint_))[set_signal_multiplexor]] >> ':'
			>> qi::uint_ >> '|'
			>> qi::uint_ >> '@'
			>> g_sym_byte_order
			>> g_sym_value_type >> '('
			>> qi::double_ >> ','
			>> qi::double_ >> ')' >> '['
			>> qi::double_ >> '|'
			>> qi::double_ >> ']'
			>> quoted_string_
			>> receivers_;
		signals_ %= *qi::attr_cast(signal_);
		message_ %=
			qi::lit("BO_")
			>> qi::uint_
			>> ident_ >> ':'
			>> qi::uint_
			>> transmitter_
			>> signals_;

		network_ %=
			*(qi::omit[version_[set_version]]
			| qi::omit[bit_timing_[set_bit_timing]]
			| qi::omit[new_symbols_[set_new_symbols]]
			| qi::omit[nodes_[insert_node_names]]
			| qi::omit[value_table_[insert_value_table_by_value_table]]
			| qi::omit[message_[insert_message_by_message]]
			| qi::omit[message_transmitter_[insert_message_transmitter_by_transmitter]]
			| qi::omit[environment_variable_[insert_environment_variable_by_environment_variable]]
			| qi::omit
				[
					  network_comment_[set_network_comment]
					| node_comment_[set_node_comment]
					| message_comment_[set_message_comment]
					| signal_comment_[set_signal_comment]
					| env_comment_[set_env_comment]
				]
			| qi::omit
				[
					  value_description_signal_[insert_value_descriptions_for_signal_by_value_descprtions]
					| value_description_env_[insert_value_descriptions_for_env_by_value_descprtions]
				]
			| qi::omit[attribute_definition_[insert_attribute_definitions_by_attribute_definition]]
			| qi::omit[attribute_default_[insert_attribute_default_by_attribute_default]]
			| qi::omit[attribute_value_ba_network_]
			| qi::omit[attribute_value_ba_node_]
			| qi::omit[attribute_value_ba_message_]
			| qi::omit[attribute_value_ba_signal_]
			)
			;

		start_ %= network_;
	}
	qi::rule<ITER, std::string()> ident_;
	qi::rule<ITER, std::string(), ascii::space_type> quoted_string_;
	qi::rule<ITER, std::set<std::string>(), ascii::space_type> receivers_;
	qi::rule<ITER, Signal(), ascii::space_type> signal_;
	qi::rule<ITER, std::map<std::string, Signal>(), ascii::space_type> signals_;
	qi::rule<ITER, Message(), ascii::space_type> message_;
	qi::rule<ITER, Network(), ascii::space_type> start_;
	qi::rule<ITER, Network(), ascii::space_type> network_;
	qi::rule<ITER, std::vector<std::string>(), ascii::space_type> new_symbols_;
	qi::rule<ITER, std::string(), ascii::space_type> version_;
	qi::rule<ITER, BitTiming(), ascii::space_type> bit_timing_;
	qi::rule<ITER, Node()> node_;
	qi::rule<ITER, std::map<std::string, Node>(), ascii::space_type> nodes_;
	qi::rule<ITER, ValueTable(), ascii::space_type> value_table_;
	qi::rule<ITER, std::string(), ascii::space_type> transmitter_;
	qi::rule<ITER, std::set<std::string>(), ascii::space_type> transmitters_;
	qi::rule<ITER, std::pair<uint32_t, std::set<std::string>>(), ascii::space_type> message_transmitter_;
	qi::rule<ITER, EnvironmentVariable(), ascii::space_type> environment_variable_;
	qi::rule<ITER, std::string(), ascii::space_type> network_comment_;
	qi::rule<ITER, std::tuple<std::string, std::string>, ascii::space_type> node_comment_;
	qi::rule<ITER, std::tuple<uint32_t, std::string>(), ascii::space_type> message_comment_;
	qi::rule<ITER, std::tuple<uint32_t, std::string, std::string>(), ascii::space_type> signal_comment_;
	qi::rule<ITER, std::tuple<std::string, std::string>(), ascii::space_type> env_comment_;
	qi::rule<ITER, std::map<uint32_t, std::string>(), ascii::space_type> value_encoding_descriptions_;
	qi::rule<ITER, std::tuple<uint32_t, std::string, std::map<uint32_t, std::string>>(), ascii::space_type> value_description_signal_;
	qi::rule<ITER, std::tuple<std::string, std::map<uint32_t, std::string>>(), ascii::space_type> value_description_env_;
	qi::rule<ITER, boost::variant<int32_t, double, std::string>(), ascii::space_type> attribute_value_;
	qi::rule<ITER, AttributeValueType(), ascii::space_type> attribute_value_type_;
	qi::rule<ITER, AttributeDefinition(), ascii::space_type> attribute_definition_;
	qi::rule<ITER, Attribute(), ascii::space_type> attribute_default_;
	qi::rule<ITER, std::tuple<Attribute>(), ascii::space_type> attribute_value_ba_network_;
	qi::rule<ITER, std::tuple<Attribute, std::string>(), ascii::space_type> attribute_value_ba_node_;
	qi::rule<ITER, std::tuple<Attribute, uint32_t>(), ascii::space_type> attribute_value_ba_message_;
	qi::rule<ITER, std::tuple<Attribute, uint32_t, std::string>(), ascii::space_type> attribute_value_ba_signal_;
	qi::rule<ITER, std::tuple<Attribute, std::string>(), ascii::space_type> attribute_value_ba_env_;
};

bool DBCPPP_EXPORT parse_dbc(const std::string& path, Vector::DBC::Network& m)
{
	bool result = false;
	std::ifstream dbc{path};
	if (dbc.is_open())
	{
		std::string str((std::istreambuf_iterator<char>(dbc)), std::istreambuf_iterator<char>());
		auto begin{str.begin()}, end{str.end()};
		NetworkGrammar<std::string::iterator> g;
		result = phrase_parse(begin, end, g, boost::spirit::ascii::space, m);
		result &= begin == end;
	}
	return result;
}
