
#include <cstdint>
#include <charconv>
#include <ostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <optional>
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

#include "DBC_Grammar.h"
#include "Network.h"
#include "NetworkImpl.h"
#include "SignalTypeImpl.h"
#include "NodeImpl.h"
#include "AttributeImpl.h"
#include "valueTableImpl.h"
#include "AttributeDefinitionImpl.h"
#include "EnvironmentVariableImpl.h"
#include "MessageImpl.h"
#include "SignalExtendedValueTypeImpl.h"

using namespace dbcppp;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace sp = boost::spirit::labels;
namespace ph = boost::phoenix;
namespace fu = boost::fusion;

BOOST_FUSION_ADAPT_STRUCT(
	NetworkImpl,
	_version,
	_new_symbols,
	_bit_timing,
	_nodes,
	_value_tables,
	_signal_types,
	_attribute_definitions,
	//attributeValues,
	//attributeRelationValues
	_signal_extended_value_types
)
BOOST_FUSION_ADAPT_STRUCT(
	NodeImpl,
	_name
)
BOOST_FUSION_ADAPT_STRUCT(
	BitTimingImpl,
	_baudrate, _BTR1, _BTR2
)
BOOST_FUSION_ADAPT_STRUCT(
	ValueTableImpl,
	_name, _value_descriptions
)

struct G_Signal
{
	std::string name;
	Signal::Multiplexer multiplexer_indicator;
	uint64_t multiplexer_switch_value;
	uint64_t start_bit;
	uint64_t bit_size;
	Signal::ByteOrder byte_order;
	Signal::ValueType value_type;
	double factor;
	double offset;
	double minimum;
	double maximum;
	std::string unit;
	std::vector<NodeImpl> receivers;
};
struct G_Message
{
	uint64_t id;
	std::string name;
	uint64_t message_size;
	NodeImpl transmitter;
	std::vector<G_Signal> signals;
};

BOOST_FUSION_ADAPT_STRUCT(
	G_Message,
	id,
	name,
	message_size,
	transmitter,
	signals
)
BOOST_FUSION_ADAPT_STRUCT(
	G_Signal,
	name,
	start_bit,
	bit_size,
	byte_order,
	value_type,
	factor,
	offset,
	minimum,
	maximum,
	unit,
	receivers
)
struct G_ValueDescriptionSignal
{
	uint64_t message_id;
	std::string signal_name;
	std::map<uint64_t, std::string> value_description;
};
struct G_ValueDescriptionEnvVar
{
	std::string env_var_name;
	std::map<uint64_t, std::string> value_description;
};
BOOST_FUSION_ADAPT_STRUCT(
	G_ValueDescriptionSignal,
	message_id,
	signal_name,
	value_description
)
BOOST_FUSION_ADAPT_STRUCT(
	G_ValueDescriptionEnvVar,
	env_var_name,
	value_description
)
struct G_MessageTransmitter
{
	uint64_t message_id;
	std::vector<std::string> transmitters;
};
BOOST_FUSION_ADAPT_STRUCT(
	G_MessageTransmitter,
	message_id,
	transmitters
)
struct G_EnvironmentVariable
{
	std::string name;
	EnvironmentVariable::VarType var_type;
	double minimum;
	double maximum;
	std::string unit;
	double initial_value;
	uint64_t ev_id;
	EnvironmentVariable::AccessType access_type;
	std::vector<std::string> access_nodes;
};
BOOST_FUSION_ADAPT_STRUCT(
	G_EnvironmentVariable,
	name,
	var_type,
	minimum,
	maximum,
	unit,
	initial_value,
	ev_id,
	access_type,
	access_nodes
)
struct G_EnvironmentVariableData
{
	std::string name;
	uint64_t data_size;
};
BOOST_FUSION_ADAPT_STRUCT(
	G_EnvironmentVariableData,
	name,
	data_size
)
BOOST_FUSION_ADAPT_STRUCT(
	SignalTypeImpl,
	_name,
	_signal_size,
	_byte_order,
	_value_type,
	_factor,
	_offset,
	_minimum,
	_maximum,
	_unit,
	_default_value,
	_value_table
)
struct G_CommentNetwork
{
	std::string comment;
};
struct G_CommentNode
{
	std::string node_name;
	std::string comment;
};
struct G_CommentMessage
{
	uint64_t message_id;
	std::string comment;
};
struct G_CommentSignal
{
	uint64_t message_id;
	std::string signal_name;
	std::string comment;
};
struct G_CommentEnvVar
{
	std::string env_var_name;
	std::string comment;
};
using variant_comment_t = boost::variant<G_CommentNetwork, G_CommentNode, G_CommentMessage, G_CommentSignal, G_CommentEnvVar>;
BOOST_FUSION_ADAPT_STRUCT(
	G_CommentNetwork,
	comment
)
BOOST_FUSION_ADAPT_STRUCT(
	G_CommentNode,
	node_name,
	comment
)
BOOST_FUSION_ADAPT_STRUCT(
	G_CommentMessage,
	message_id,
	comment
)
BOOST_FUSION_ADAPT_STRUCT(
	G_CommentSignal,
	message_id,
	signal_name,
	comment
)
BOOST_FUSION_ADAPT_STRUCT(
	G_CommentEnvVar,
	env_var_name,
	comment
)
BOOST_FUSION_ADAPT_STRUCT(
	AttributeDefinitionImpl,
	_object_type,
	_name,
	_value_type
)
BOOST_FUSION_ADAPT_STRUCT(
	AttributeDefinition::ValueTypeInt,
	minimum,
	maximum
)
BOOST_FUSION_ADAPT_STRUCT(
	AttributeDefinition::ValueTypeHex,
	minimum,
	maximum
)
BOOST_FUSION_ADAPT_STRUCT(
	AttributeDefinition::ValueTypeFloat,
	minimum,
	maximum
)
BOOST_FUSION_ADAPT_STRUCT(
	AttributeDefinition::ValueTypeEnum,
	values
)
using attr_value_t = boost::variant<uint64_t, int64_t, double, std::string>;
struct G_Attribute
{
	std::string name;
	attr_value_t value;
};
BOOST_FUSION_ADAPT_STRUCT(
	G_Attribute,
	name,
	value
)
struct G_AttributeNetwork
{
	std::string attribute_name;
	attr_value_t value;
};
struct G_AttributeNode
{
	std::string attribute_name;
	std::string node_name;
	attr_value_t value;
};
struct G_AttributeMessage
{
	std::string attribute_name;
	uint64_t message_id;
	attr_value_t value;
};
struct G_AttributeSignal
{
	std::string attribute_name;
	uint64_t message_id;
	std::string signal_name;
	attr_value_t value;
};
struct G_AttributeEnvVar
{
	std::string attribute_name;
	std::string env_var_name;
	attr_value_t value;
};
using variant_attribute_t = boost::variant<G_AttributeNetwork, G_AttributeNode, G_AttributeMessage, G_AttributeSignal, G_AttributeEnvVar>;
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeNetwork,
	attribute_name,
	value
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeNode,
	attribute_name,
	node_name,
	value
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeMessage,
	attribute_name,
	message_id,
	value
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeSignal,
	attribute_name,
	message_id,
	signal_name,
	value
)
BOOST_FUSION_ADAPT_STRUCT(
	G_AttributeEnvVar,
	attribute_name,
	env_var_name,
	value
)
/*
struct G_SignalExtendedValueType{
    uint64_t message_id;
    std::string signal_name;
    uint64_t value;
};
*/
BOOST_FUSION_ADAPT_STRUCT(
    SignalExtendedValueTypeImpl,
    _message_id,
    _signal_name,
    _value
)

auto set_bit_timing =
	[](const boost::optional<fu::vector<uint64_t, uint64_t, uint64_t>>& timing, auto& context)
	{
		BitTimingImpl& bit_timing = context.attributes.car;
		bit_timing._baudrate = 0;
		bit_timing._BTR1 = 0;
		bit_timing._BTR2 = 0;
		if (timing)
		{
			bit_timing._baudrate = fu::at_c<0>(*timing);
			bit_timing._BTR1 = fu::at_c<1>(*timing);
			bit_timing._BTR2 = fu::at_c<2>(*timing);
		}
	};
auto node_to_pair =
	[](const std::string& name, auto& context)
	{
		context.attributes.car.first = name;
		context.attributes.car.second._name = name;
	};
auto set_value_table =
	[](const fu::vector<std::string, std::map<uint64_t, std::string>>& table, auto& context)
	{
		context.attributes.car.first = fu::at_c<0>(table);
		context.attributes.car.second._name = fu::at_c<0>(table);
		context.attributes.car.second._value_descriptions = fu::at_c<1>(table);
	};
auto set_multiplexor =
	[](const boost::optional<std::string>& m, auto& context)
	{
		if (m)
		{
			if (*m == "M")
			{
				context.attributes.car.multiplexer_indicator = Signal::Multiplexer::MuxSwitch;
			}
			else
			{
				context.attributes.car.multiplexer_indicator = Signal::Multiplexer::MuxValue;
				std::string_view buff{m->c_str() + 1, m->size() - 1};
				std::from_chars(m->c_str() + 1, m->c_str() + m->size(), context.attributes.car.multiplexer_switch_value);
			}
		}
		else
		{
			context.attributes.car.multiplexer_indicator = Signal::Multiplexer::NoMux;
		}
	};
auto set_byte_order =
	[](char chr, auto& context)
	{
		switch (chr)
		{
		case '0': context.attributes.car = Signal::ByteOrder::BigEndian; break;
		case '1': context.attributes.car = Signal::ByteOrder::LittleEndian; break;
		}
	};
auto set_value_type =
	[](char chr, auto& context)
	{
		switch (chr)
		{
		case '-': context.attributes.car = Signal::ValueType::Signed; break;
		case '+': context.attributes.car = Signal::ValueType::Unsigned; break;
		}
	};
auto set_node_name =
	[](const std::string& node, auto& context)
	{
		context.attributes.car = node == "Vector__XXX" ? "" : node;
	};
auto set_env_var_type =
	[](const uint64_t env_var_type, auto& context)
	{
		switch (env_var_type)
		{
		case 0: context.attributes.car = EnvironmentVariable::VarType::Integer;
		case 1: context.attributes.car = EnvironmentVariable::VarType::Float;
		case 2: context.attributes.car = EnvironmentVariable::VarType::String;
		}
	};
auto set_access_type =
	[](const std::string& access_type, auto& context)
	{
		if (access_type == "DUMMY_NODE_VECTOR0")
		{
			context.attributes.car = EnvironmentVariable::AccessType::Unrestricted;
		}
		else if (access_type == "DUMMY_NODE_VECTOR1")
		{
			context.attributes.car = EnvironmentVariable::AccessType::Unrestricted;
		}
		else if (access_type == "DUMMY_NODE_VECTOR2")
		{
			context.attributes.car = EnvironmentVariable::AccessType::Unrestricted;
		}
		else if (access_type == "DUMMY_NODE_VECTOR3")
		{
			context.attributes.car = EnvironmentVariable::AccessType::Unrestricted;
		}
	};
auto signal_type_to_pair =
	[](const SignalTypeImpl& sig_type, auto& context)
	{
		context.attributes.car.first  = sig_type._name;
		context.attributes.car.second = sig_type;
	};
auto optional_string_to_object_type =
	[](const boost::optional<std::string>& g_obj_type, auto& context)
	{
		context.attributes.car = AttributeDefinition::ObjectType::Network;
		if (g_obj_type)
		{
			if (*g_obj_type == "BU_")
			{
				context.attributes.car = AttributeDefinition::ObjectType::Node;
			}
			else if (*g_obj_type == "BO_")
			{
				context.attributes.car = AttributeDefinition::ObjectType::Message;
			}
			else if (*g_obj_type == "SG_")
			{
				context.attributes.car = AttributeDefinition::ObjectType::Signal;
			}
			else if (*g_obj_type == "EV_")
			{
				context.attributes.car = AttributeDefinition::ObjectType::EnvironmentVariable;
			}
		}
	};
auto attribute_definition_to_pair =
	[](const AttributeDefinitionImpl& attr_def, auto& context)
	{
		context.attributes.car.first  = attr_def._name;
		context.attributes.car.second = attr_def;
	};
auto insert_nodes_into_network =
	[](const std::vector<NodeImpl>& g_nodes, auto& context)
	{
		auto& net = context.attributes.car;
		for (const auto& g_n : g_nodes)
		{
			net->_nodes.insert(std::make_pair(g_n.name, NodeImpl()));
		}
	};
auto insert_messages_into_network =
	[](const std::vector<G_Message>& g_msgs, auto& context)
	{
		auto& net = context.attributes.car;
		for (const auto& g_msg : g_msgs)
		{
			MessageImpl& msg = net._messages[g_msg.id];
			msg._id           = g_msg.id;
			msg._name         = g_msg.name;
			msg._message_size = g_msg.message_size;
			if (g_msg.transmitter._name != "")
			{
				msg._transmitter  = &net._nodes[g_msg.transmitter._name];
			}
			for (const auto& g_sig : g_msg.signals)
			{
				SignalImpl sig(g_sig.byte_order, g_sig.value_type, g_sig.bit_size, g_sig.start_bit, msg._message_size);
				sig._parent_message           = &msg;
				sig._name                     = g_sig.name;
				sig._factor                   = g_sig.factor;
				sig._offset                   = g_sig.offset;
				sig._minimum                  = g_sig.minimum;
				sig._unit                     = g_sig.unit;
				sig._multiplexer_indicator    = g_sig.multiplexer_indicator;
				sig._multiplexer_switch_value = g_sig.multiplexer_switch_value;
				for (const auto& n : g_sig.receivers)
				{
					if (n._name != "")
					{
						sig._receivers.insert(&net._nodes[n._name]);
					}
				}
				msg._signals.insert(std::make_pair(sig._name, std::move(sig)));
			}
		}
	};
auto insert_message_transmitters_into_network =
	[](const std::vector<G_MessageTransmitter>& message_transmitters, auto& context)
	{
		NetworkImpl& net = context.attributes.car;
		for (const auto& msg_trans : message_transmitters)
		{
			MessageImpl& msg = net._messages[msg_trans.message_id];
			for (const auto& trans : msg_trans.transmitters)
			{
				msg._transmitters.insert(&net._nodes[trans]);
			}
		}
	};
auto insert_environment_variables_into_network =
	[](const std::vector<G_EnvironmentVariable>& g_env_vars, auto& context)
	{
		NetworkImpl& net = context.attributes.car;
		for (const auto& g_env_var : g_env_vars)
		{
			EnvironmentVariableImpl& env_var = net._environment_variables[g_env_var.name];
			env_var._name          = g_env_var.name;
			env_var._var_type      = g_env_var.var_type;
			env_var._minimum       = g_env_var.minimum;
			env_var._maximum       = g_env_var.maximum;
			env_var._unit          = g_env_var.unit;
			env_var._initial_value = g_env_var.initial_value;
			env_var._ev_id         = g_env_var.ev_id;
			env_var._access_type   = g_env_var.access_type;
			for (const auto& n : g_env_var.access_nodes)
			{
				env_var._access_nodes.insert(&net._nodes[n]);
			}
		}
	};
auto insert_environment_variable_datas_into_network =
	[](const std::vector<G_EnvironmentVariableData>& g_env_datas, auto& context)
	{
		NetworkImpl& net = context.attributes.car;
		for (const auto& g_env_data : g_env_datas)
		{
			EnvironmentVariableImpl& env_var = net._environment_variables[g_env_data.name];
			env_var._var_type  = EnvironmentVariable::VarType::Data;
			env_var._data_size = g_env_data.data_size;
		}
	};
auto insert_comments_into_network =
	[](const std::vector<variant_comment_t>& g_comments, auto& context)
	{
		NetworkImpl& net = context.attributes.car;
		struct Visitor
		{
			Visitor(NetworkImpl& net)
				: _net{net}
			{}
			void operator()(const G_CommentNetwork& c) const
			{
				_net._comment = c.comment;
			}
			void operator()(const G_CommentNode& c) const
			{
				_net._nodes[c.node_name]._comment = c.comment;
			}
			void operator()(const G_CommentMessage& c) const
			{
				_net._messages[c.message_id]._comment = c.comment;
			}
			void operator()(const G_CommentSignal& c) const
			{
				_net._messages[c.message_id]._signals[c.signal_name]._comment = c.comment;
			}
			void operator()(const G_CommentEnvVar& c) const
			{
				_net._environment_variables[c.env_var_name]._comment = c.comment;
			}
			NetworkImpl& _net;
		};
		for (const auto& comment : g_comments)
		{
			boost::apply_visitor(Visitor{net}, comment);
		}
	};
auto insert_attribute_defaults_into_network =
	[](const std::vector<G_Attribute>& g_attrs, auto& context)
	{
		NetworkImpl& net = context.attributes.car;
		for (const auto& g_attr : g_attrs)
		{
			AttributeDefinitionImpl& attr_def = net._attribute_definitions[g_attr.name];
			AttributeImpl& attr = net._attribute_defaults[g_attr.name];
			attr._name = g_attr.name;
			attr._object_type = attr_def._object_type;
			switch (attr_def._value_type.which())
			{
			case 0: attr._value = Attribute::IntegerValue { boost::get<int64_t>     (g_attr.value) }; break;
			case 1: attr._value = Attribute::HexValue     { boost::get<int64_t>     (g_attr.value) }; break;
			case 2: attr._value = Attribute::FloatValue   { boost::get<double>      (g_attr.value) }; break;
			case 3: attr._value = Attribute::StringValue  { boost::get<std::string> (g_attr.value) }; break;
			case 4: attr._value = Attribute::StringValue  { boost::get<std::string> (g_attr.value) }; break;
			}
		}
	};
auto insert_attribute_values_into_network =
	[](const std::vector<variant_attribute_t>& g_attrs, auto& context)
	{
		NetworkImpl& net = context.attributes.car;
		struct Visitor
		{
			Visitor(NetworkImpl& net)
				: _net{net}
			{}
			void assign(const std::string& attr_name, const attr_value_t& value, AttributeImpl& attr)
			{
				AttributeDefinitionImpl& attr_def = _net._attribute_definitions[attr_name];
				attr._name = attr_name;
				attr._object_type = AttributeDefinition::ObjectType::Network;
				switch (attr_def._value_type.which())
				{
					case 0: attr._value = Attribute::IntegerValue { boost::get<int64_t>     (value) }; break;
					case 1: attr._value = Attribute::HexValue     { boost::get<int64_t>     (value) }; break;
					case 2: attr._value = Attribute::FloatValue   { boost::get<double>      (value) }; break;
					case 3: attr._value = Attribute::StringValue  { boost::get<std::string> (value) }; break;
					case 4: attr._value = Attribute::EnumValue    { boost::get<int64_t>     (value) }; break;
				}
			}
			void operator()(const G_AttributeNetwork& g_attr)
			{
				AttributeImpl& attr = _net._attribute_values[g_attr.attribute_name];
				assign(g_attr.attribute_name, g_attr.value, attr);
			}
			void operator()(const G_AttributeNode& g_attr)
			{
				AttributeImpl& attr = _net._nodes[g_attr.node_name]._attribute_values[g_attr.attribute_name];
				assign(g_attr.attribute_name, g_attr.value, attr);
			}
			void operator()(const G_AttributeMessage& g_attr)
			{
				AttributeImpl& attr = _net._messages[g_attr.message_id]._attribute_values[g_attr.attribute_name];
				assign(g_attr.attribute_name, g_attr.value, attr);
			}
			void operator()(const G_AttributeSignal& g_attr)
			{
				AttributeImpl& attr = _net._messages[g_attr.message_id]._signals[g_attr.signal_name]._attribute_values[g_attr.attribute_name];
				assign(g_attr.attribute_name, g_attr.value, attr);
			}
			void operator()(const G_AttributeEnvVar& g_attr)
			{
				AttributeImpl& attr = _net._environment_variables[g_attr.env_var_name]._attribute_values[g_attr.attribute_name];
				assign(g_attr.attribute_name, g_attr.value, attr);
			}
			NetworkImpl& _net;
		};
		for (const auto& g_attr : g_attrs)
		{
			boost::apply_visitor(Visitor{net}, g_attr);
		}
	};
auto insert_value_descriptions_into_network =
	[](const std::vector<boost::variant<G_ValueDescriptionSignal, G_ValueDescriptionEnvVar>>& value_descriptions, auto& context)
	{
		NetworkImpl& net = context.attributes.car;
		struct Visitor
		{
			Visitor(NetworkImpl& net)
				: _net{net}
			{}
			void operator()(const G_ValueDescriptionSignal& desc)
			{
				_net._messages[desc.message_id]._signals[desc.signal_name]._value_descriptions = std::move(desc.value_description);
			}
			void operator()(const G_ValueDescriptionEnvVar& desc)
			{
				_net._environment_variables[desc.env_var_name]._value_descriptions = std::move(desc.value_description);
			}
			NetworkImpl& _net;
		};
		for (const auto& desc : value_descriptions)
		{
			boost::apply_visitor(Visitor(net), desc);
		}
	};

template <class Iter>
struct NetworkGrammar
	: public qi::grammar<Iter, NetworkImpl(), ascii::space_type>
{
	using Skipper = ascii::space_type;
	NetworkGrammar()
		: NetworkGrammar::base_type(_DBC_file)
	{
		_DBC_file %= 
			   _version
			>> _new_symbols
			>> _bit_timing
			>> _nodes
			>> _value_tables
			>> qi::omit[_messages[insert_messages_into_network]]
			>> qi::omit[_message_transmitters[insert_message_transmitters_into_network]]
			>> qi::omit[_environment_variables[insert_environment_variables_into_network]]
			>> qi::omit[_environment_variable_datas[insert_environment_variable_datas_into_network]]
			>> _signal_types
			>> qi::omit[_comments[insert_comments_into_network]]
			>> _attribute_definitions
			>> qi::omit[_attribute_defaults[insert_attribute_defaults_into_network]]
			>> qi::omit[_attribute_values[insert_attribute_values_into_network]]
			>> qi::omit[_value_descriptions[insert_value_descriptions_into_network]]
            >> _signal_extended_value_types;

		_unsigned_integer %= qi::uint_;
		_signed_integer %= qi::int_;
		_double %= qi::double_;
		_char_string %= qi::lexeme['"' >> *(('\\' >> qi::char_("\\\"")) | ~qi::char_('"')) >> '"'];
		_C_identifier %= qi::lexeme[qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9")];
		_C_identifier_ %= qi::lexeme[qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9")];

		_version %= qi::lit("VERSION") >> _char_string;
		_new_symbol %= qi::lexeme[
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
		_new_symbols %= qi::lit("NS_") >> ':' >> *_new_symbol;
		_new_symbol_values %= -*_C_identifier;
		
		_bit_timing = qi::lit("BS_") >> ':' >> qi::omit[(-(_baudrate >> ':' >> _BTR1 >> ',' >> _BTR2))[set_bit_timing]];
		_baudrate %= _unsigned_integer;
		_BTR1 %= _unsigned_integer;
		_BTR2 %= _unsigned_integer;
		
		_nodes %= qi::lit("BU_") >> ':' >> qi::skip(ascii::blank)[*_node_ >> qi::eol];
		_node_ = _node_name_[node_to_pair];
		_node_name = _C_identifier[set_node_name];
		_node_name_ = _C_identifier_[set_node_name];

		_value_tables %= *_value_table;
		_value_table = qi::lit("VAL_TABLE_") >> (_value_table_name >> _value_encoding_descriptions)[set_value_table];
		_value_table_name %= _C_identifier;
		_value_encoding_descriptions %= *_value_encoding_description;
		_value_encoding_description %= _double >> _char_string;

		_messages %= *_message;
		_message %= qi::lit("BO_") >> _message_id >> _message_name >> ':' >> _message_size >> _transmitter >> _signals;
		_message_id %= _unsigned_integer;
		_message_name %= _C_identifier;
		_message_size %= _unsigned_integer;
		_transmitter %= _node_name;

		_signals %= *_signal;
		_signal %= qi::lit("SG_") >> _signal_name >> qi::omit[_multiplexer_indicator[set_multiplexor]] >> ':' >> _start_bit >> '|'
			>> _signal_size >> '@' >> _byte_order >> _value_type >> '('
			>> _factor >> ',' >> _offset >> ')' >> '[' >> _minimum >> '|' >> _maximum >> ']' >> _unit >> _receivers;
		_signal_name %= _C_identifier;
		_multiplexer_indicator %= -_C_identifier;
		_start_bit %= _unsigned_integer;
		_signal_size %= _unsigned_integer;
		_byte_order = qi::lexeme[qi::char_('0') | qi::char_('1')][set_byte_order];
		_value_type = qi::lexeme[qi::char_('-') | qi::char_('+')][set_value_type];
		_factor %= _double;
		_offset %= _double;
		_minimum %= _double;
		_maximum %= _double;
		_unit %= _char_string;
		_receivers = qi::skip(ascii::blank)[*_receiver_ >> qi::eol];
		_receiver_ %= _node_name_;

		_message_transmitters %= *_message_transmitter;
		_message_transmitter %= qi::lit("BO_TX_BU_") >> _message_id >> ':' >> _transmitters >> ';';
		_transmitters %= _transmitter % ',';

		_environment_variables %= *_environment_variable;
		_environment_variable %= qi::lit("EV_") >> _env_var_name >> ':' >> _env_var_type >> '[' >> _minimum >> '|' >> _maximum >> ']'
			>> _unit >> _initial_value >> _ev_id >> _access_type >> _access_nodes >> ';';
		_env_var_name %= _C_identifier;
		_env_var_type %= _unsigned_integer[set_env_var_type];
		_initial_value %= _double;
		_ev_id %= _unsigned_integer;
		_access_type = _C_identifier[set_access_type];
		_access_nodes = _node_name % ',';

		_environment_variable_datas %= *_environment_variable_data;
		_environment_variable_data %= qi::lit("ENVVAR_DATA_") >> _env_var_name >> ':' >> _data_size >> ';';
		_data_size %= _unsigned_integer;

		_signal_types %= *_signal_type_inter;
		_signal_type_inter = _signal_type[signal_type_to_pair];
		_signal_type %= qi::lit("SGTYPE_") >> _signal_type_name >> ':' >> _signal_size >> '@'
			>> _byte_order >> _value_type >> '(' >> _factor >> ',' >> _offset >> ')' >> '['
			>> _minimum >> '|' >> _maximum >> ']' >> _unit >> _default_value >> ',' >> _value_table_name >> ';';
		_signal_type_name %= _C_identifier;
		_default_value %= _double;
		
		_comments %= *_comment;
		_comment %= qi::lit("CM_") >> (_comment_node | _comment_message | _comment_signal | _comment_env_var | _comment_network);
		_comment_network %= _char_string >> ';';
		_comment_node %= qi::lit("BU_") >> _node_name >> _char_string >> ';';
		_comment_message %= qi::lit("BO_") >> _message_id >> _char_string >> ';';
		_comment_signal %= qi::lit("SG_") >> _message_id >> _signal_name >> _char_string >> ';';
		_comment_env_var %= qi::lit("EV_") >> _env_var_name >> _char_string >> ';';

		_attribute_definitions %= *(_attribute_definition_inter);
		_attribute_definition_inter = _attribute_definition[attribute_definition_to_pair];
		_attribute_definition %= qi::lit("BA_DEF_") >> _object_type >> _attribute_name >> _attribute_value_type >> ';';
		_object_type = (-(qi::string("BU_") | qi::string("BO_") | qi::string("SG_") | qi::string("EV_")))[optional_string_to_object_type];
		_attribute_name %= _char_string;
		_attribute_value_type %= _attribute_value_type_int | _attribute_value_type_hex | _attribute_value_type_float
			| _attribute_value_type_string | _attribute_value_type_enum;
		_attribute_value_type_int %= qi::lit("INT") >> _signed_integer >> _signed_integer;
		_attribute_value_type_hex %= qi::lit("HEX") >> _signed_integer >> _signed_integer;
		_attribute_value_type_float %= qi::lit("FLOAT") >> _double >> _double;
		auto drain = [](const auto& p1, auto p2) {};
		_attribute_value_type_string = qi::lit("STRING")[drain];
		_attribute_value_type_enum %= qi::lit("ENUM") >> (_char_string % ',');

		_attribute_defaults %= *_attribute_default;
		_attribute_default %= (qi::lit("BA_DEF_DEF_REL_") | qi::lit("BA_DEF_DEF_"))
			>> _attribute_name >> _attribute_value >> ';';
		_attribute_value %= _signed_integer | _double | _char_string;

		_attribute_values %= *_attribute_value_ent;
		_attribute_value_ent %= qi::lit("BA_")
			>> (_attribute_value_ent_network | _attribute_value_ent_node
			  | _attribute_value_ent_message | _attribute_value_ent_signal
			  | _attribute_value_ent_env_var) >> ';';
		_attribute_value_ent_network %= _attribute_name >> _attribute_value;
		_attribute_value_ent_node %= _attribute_name >> qi::lit("BU_") >> _node_name >> _attribute_value;
		_attribute_value_ent_message %= _attribute_name >> qi::lit("BO_") >> _message_id >> _attribute_value;
		_attribute_value_ent_signal %= _attribute_name >> qi::lit("SG_") >> _message_id >> _signal_name >> _attribute_value;
		_attribute_value_ent_env_var %= _attribute_name >> qi::lit("EV_") >> _env_var_name >> _attribute_value;

		_value_descriptions %= *_value_description_sig_env_var;
		_value_description_sig_env_var %= _value_description_signal | _value_description_env_var;
		_value_description_signal %= qi::lit("VAL_") >> _message_id >> _signal_name >> _value_encoding_descriptions >> ';';
		_value_description_env_var %= qi::lit("VAL_") >> _env_var_name >> _value_encoding_descriptions >> ';';

        _signal_extended_value_types %= *_signal_extended_value_type;
        _signal_extended_value_type %= qi::lit("SIG_VALTYPE_") >> _message_id >> _signal_name >> ':' >>  _unsigned_integer >> ';';
	}

	qi::rule<Iter, NetworkImpl(), Skipper> _DBC_file;
	
	qi::rule<Iter, uint64_t(), Skipper> _unsigned_integer;
	qi::rule<Iter, int64_t(), Skipper> _signed_integer;
	qi::rule<Iter, double(), Skipper> _double;
	qi::rule<Iter, std::string(), Skipper> _char_string;
	qi::rule<Iter, std::string(), Skipper> _C_identifier;
	qi::rule<Iter, std::string(), ascii::blank_type> _C_identifier_;
	
	qi::rule<Iter, std::string(), Skipper> _version;
	qi::rule<Iter, std::string(), Skipper> _new_symbol;
	qi::rule<Iter, std::vector<std::string>(), Skipper> _new_symbols;
	qi::rule<Iter, std::vector<std::string>(), Skipper> _new_symbol_values;
	qi::rule<Iter, BitTimingImpl(), Skipper> _bit_timing;
	qi::rule<Iter, uint64_t(), Skipper> _baudrate;
	qi::rule<Iter, uint64_t(), Skipper> _BTR1;
	qi::rule<Iter, uint64_t(), Skipper> _BTR2;

	qi::rule<Iter, std::map<std::string, NodeImpl>(), Skipper> _nodes;
	qi::rule<Iter, std::string(), Skipper> _node_name;
	qi::rule<Iter, std::pair<std::string, NodeImpl>(), ascii::blank_type> _node_;
	qi::rule<Iter, std::vector<std::string>(), ascii::blank_type> _node_names;
	qi::rule<Iter, std::string(), ascii::blank_type> _node_name_;

	qi::rule<Iter, std::map<std::string, ValueTableImpl>(), Skipper> _value_tables;
	qi::rule<Iter, std::pair<std::string, ValueTableImpl>(), Skipper> _value_table;
	qi::rule<Iter, std::string(), Skipper> _value_table_name;
	qi::rule<Iter, std::map<uint64_t, std::string>(), Skipper> _value_encoding_descriptions;
	qi::rule<Iter, std::pair<uint64_t, std::string>(), Skipper> _value_encoding_description;

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
	qi::rule<Iter, Signal::ByteOrder(), Skipper> _byte_order;
	qi::rule<Iter, Signal::ValueType(), Skipper> _value_type;
	qi::rule<Iter, double(), Skipper> _factor;
	qi::rule<Iter, double(), Skipper> _offset;
	qi::rule<Iter, double(), Skipper> _maximum;
	qi::rule<Iter, double(), Skipper> _minimum;
	qi::rule<Iter, std::string(), Skipper> _unit;
	qi::rule<Iter, std::vector<NodeImpl>(), Skipper> _receivers;
	qi::rule<Iter, NodeImpl(), ascii::blank_type> _receiver_;

	qi::rule<Iter, std::vector<G_MessageTransmitter>(), Skipper> _message_transmitters;
	qi::rule<Iter, G_MessageTransmitter(), Skipper> _message_transmitter;
	qi::rule<Iter, std::vector<std::string>(), Skipper> _transmitters;

	qi::rule<Iter, std::vector<G_EnvironmentVariable>(), Skipper> _environment_variables;
	qi::rule<Iter, G_EnvironmentVariable(), Skipper> _environment_variable;
	qi::rule<Iter, std::string(), Skipper> _env_var_name;
	qi::rule<Iter, EnvironmentVariable::VarType(), Skipper> _env_var_type;
	qi::rule<Iter, double(), Skipper> _initial_value;
	qi::rule<Iter, uint64_t(), Skipper> _ev_id;
	qi::rule<Iter, EnvironmentVariable::AccessType(), Skipper> _access_type;
	qi::rule<Iter, std::vector<std::string>(), Skipper> _access_nodes;
	
	qi::rule<Iter, std::vector<G_EnvironmentVariableData>(), Skipper> _environment_variable_datas;
	qi::rule<Iter, G_EnvironmentVariableData(), Skipper> _environment_variable_data;
	qi::rule<Iter, uint64_t(), Skipper> _data_size;
	
	qi::rule<Iter, std::map<std::string, SignalTypeImpl>(), Skipper> _signal_types;
	qi::rule<Iter, std::pair<std::string, SignalTypeImpl>(), Skipper> _signal_type_inter;
	qi::rule<Iter, SignalTypeImpl(), Skipper> _signal_type;
	qi::rule<Iter, std::string(), Skipper> _signal_type_name;
	qi::rule<Iter, double(), Skipper> _default_value;

	qi::rule<Iter, std::vector<variant_comment_t>(), Skipper> _comments;
	qi::rule<Iter, variant_comment_t(), Skipper> _comment;
	qi::rule<Iter, G_CommentNetwork(), Skipper> _comment_network;
	qi::rule<Iter, G_CommentNode(), Skipper> _comment_node;
	qi::rule<Iter, G_CommentMessage(), Skipper> _comment_message;
	qi::rule<Iter, G_CommentSignal(), Skipper> _comment_signal;
	qi::rule<Iter, G_CommentEnvVar(), Skipper> _comment_env_var;

	qi::rule<Iter, std::map<std::string, AttributeDefinitionImpl>(), Skipper> _attribute_definitions;
	qi::rule<Iter, std::pair<std::string, AttributeDefinitionImpl>(), Skipper> _attribute_definition_inter;
	qi::rule<Iter, AttributeDefinitionImpl(), Skipper> _attribute_definition;
	qi::rule<Iter, AttributeDefinition::ObjectType(), Skipper> _object_type;
	qi::rule<Iter, std::string(), Skipper> _attribute_name;
	qi::rule<Iter, AttributeDefinition::value_type_t(), Skipper> _attribute_value_type;
	qi::rule<Iter, AttributeDefinition::ValueTypeInt(), Skipper> _attribute_value_type_int;
	qi::rule<Iter, AttributeDefinition::ValueTypeHex(), Skipper> _attribute_value_type_hex;
	qi::rule<Iter, AttributeDefinition::ValueTypeFloat(), Skipper> _attribute_value_type_float;
	qi::rule<Iter, AttributeDefinition::ValueTypeString(), Skipper> _attribute_value_type_string;
	qi::rule<Iter, AttributeDefinition::ValueTypeEnum(), Skipper> _attribute_value_type_enum;

	qi::rule<Iter, std::vector<G_Attribute>(), Skipper> _attribute_defaults;
	qi::rule<Iter, G_Attribute(), Skipper> _attribute_default;
	qi::rule<Iter, boost::variant<int64_t, double, std::string>(), Skipper> _attribute_value;

	qi::rule<Iter, std::vector<variant_attribute_t>(), Skipper> _attribute_values;
	qi::rule<Iter, variant_attribute_t(), Skipper> _attribute_value_ent;
	qi::rule<Iter, G_AttributeNetwork(), Skipper> _attribute_value_ent_network;
	qi::rule<Iter, G_AttributeNode(), Skipper> _attribute_value_ent_node;
	qi::rule<Iter, G_AttributeMessage(), Skipper> _attribute_value_ent_message;
	qi::rule<Iter, G_AttributeSignal(), Skipper> _attribute_value_ent_signal;
	qi::rule<Iter, G_AttributeEnvVar(), Skipper> _attribute_value_ent_env_var;

	qi::rule<Iter, std::vector<boost::variant<G_ValueDescriptionSignal, G_ValueDescriptionEnvVar>>(), Skipper> _value_descriptions;
	qi::rule<Iter, boost::variant<G_ValueDescriptionSignal, G_ValueDescriptionEnvVar>(), Skipper> _value_description_sig_env_var;
	qi::rule<Iter, G_ValueDescriptionSignal(), Skipper> _value_description_signal;
	qi::rule<Iter, G_ValueDescriptionEnvVar(), Skipper> _value_description_env_var;

    qi::rule<Iter, std::vector<SignalExtendedValueTypeImpl>(), Skipper> _signal_extended_value_types;
    qi::rule<Iter, SignalExtendedValueTypeImpl(), Skipper> _signal_extended_value_type;
};

DBCPPP_API bool operator>>(std::istream& is, Network& net)
{
	bool result = false;
	std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
	auto begin{str.begin()}, end{str.end()};
	NetworkGrammar<std::string::iterator> g;
	result = phrase_parse(begin, end, g, ascii::space, static_cast<NetworkImpl&>(net));
	if (begin != end)
	{
		std::cout << std::string(begin, end) << std::endl;
	}
 	return result & (begin == end);
}