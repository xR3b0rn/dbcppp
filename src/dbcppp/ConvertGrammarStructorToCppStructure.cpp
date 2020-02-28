
#include "NetworkImpl.h"
#include "DBC_Grammar.h"
#include "ConvertGrammarStructorToCppStructure.h"
#include <boost/log/trivial.hpp>

using namespace dbcppp;

void dbcppp::ConvertGrammarStructorToCppStructure(const G_Network& gnet, NetworkImpl& net)
{
	net._version = gnet.version.version;
	for (auto& ns : gnet.new_symbols)
	{
		net._new_symbols.emplace_back(ns);
	}
	net._bit_timing._baudrate = 0;
	net._bit_timing._BTR1 = 0;
	net._bit_timing._BTR2 = 0;
	if (gnet.bit_timing)
	{
		net._bit_timing._baudrate = gnet.bit_timing->baudrate;
		net._bit_timing._BTR1 = gnet.bit_timing->BTR2;
		net._bit_timing._BTR2 = gnet.bit_timing->BTR2;
	}
	for (auto& n : gnet.nodes)
	{
		NodeImpl& nn = net._nodes[n.name];
		nn._name = n.name;
	}
	for (auto& vt : gnet.value_tables)
	{
		ValueTableImpl& nvt = net._value_tables[vt.name];
		nvt._name = vt.name;
		for (auto& vd : vt.value_encoding_descriptions)
		{
			nvt._value_descriptions.insert(std::make_pair(vd.first, vd.second));
		}
	}
	for (auto& m : gnet.messages)
	{
		MessageImpl& nm = net._messages[m.id];
		nm._id = m.id;
		nm._name = m.name;
		nm._message_size = m.size;
		nm._transmitter = m.transmitter;
		for (auto& s : m.signals)
		{
			SignalImpl ns(
				  s.byte_order == '0' ? Signal::ByteOrder::BigEndian : Signal::ByteOrder::LittleEndian
				, s.value_type == '+' ? Signal::ValueType::Unsigned : Signal::ValueType::Signed
				, s.signal_size, s.start_bit, m.size);
			if (ns.getError() == SignalImpl::ErrorCode::SignalExceedsMessageSize)
			{
				BOOST_LOG_TRIVIAL(warning) << "The signals '" << m.name << "::" << s.name << "'" << " start_bit + bit_size exceeds the byte size of the message! Ignoring this error will lead to garbage data when using the decode function of this signal.";
			}
			ns._factor = s.factor;
			ns._minimum = s.minimum;
			ns._maximum = s.maximum;
			ns._multiplexer_switch_value = 0;
			ns._bit_size = s.signal_size;
			if (s.multiplexer_indicator)
			{
				auto m = *s.multiplexer_indicator;
				if (m.substr(0, 1) == "M")
				{
					ns._multiplexer_indicator = Signal::Multiplexer::MuxSwitch;
				}
				else
				{
					ns._multiplexer_indicator = Signal::Multiplexer::MuxValue;
					std::string value = m.substr(1, m.size());
					ns._multiplexer_switch_value = std::atoi(value.c_str());
				}
			}
			ns._name = s.name;
			for (auto& n : s.receivers)
			{
				ns._receivers.insert(n.name);
			}
			ns._offset = s.offset;
			ns._unit = s.unit;
			nm._signals.insert(std::make_pair(ns._name, std::move(ns)));
		}
	}
	for (auto& ev : gnet.environment_variables)
	{
		EnvironmentVariableImpl& nev = net._environment_variables[ev.name];
		for (auto& an : ev.access_nodes)
		{
			nev._access_nodes.insert(an.name);
		}
		if (ev.access_type == "DUMMY_NODE_VECTOR0")
		{
			nev._access_type = EnvironmentVariable::AccessType::Unrestricted;
		}
		else if (ev.access_type == "DUMMY_NODE_VECTOR1")
		{
			nev._access_type = EnvironmentVariable::AccessType::Unrestricted;
		}
		else if (ev.access_type == "DUMMY_NODE_VECTOR2")
		{
			nev._access_type = EnvironmentVariable::AccessType::Unrestricted;
		}
		else if (ev.access_type == "DUMMY_NODE_VECTOR3")
		{
			nev._access_type = EnvironmentVariable::AccessType::Unrestricted;
		}
		nev._ev_id = ev.id;
		nev._initial_value = ev.initial_value;
		nev._maximum = ev.maximum;
		nev._minimum = ev.minimum;
		nev._name = ev.name;
		nev._unit = ev.unit;
		switch (ev.var_type)
		{
		case 0: nev._var_type = EnvironmentVariable::VarType::Integer; break;
		case 1: nev._var_type = EnvironmentVariable::VarType::Float; break;
		case 2: nev._var_type = EnvironmentVariable::VarType::String; break;
		}
	}
	for (auto& evd : gnet.environment_variable_datas)
	{
		EnvironmentVariableImpl& nev = net._environment_variables[evd.name];
		nev._var_type = EnvironmentVariable::VarType::Data;
		nev._data_size = evd.size;
	}
	for (auto& st : gnet.signal_types)
	{
		SignalTypeImpl& nst = net._signal_types[st.name];
		nst._byte_order = st.byte_order == '0' ? Signal::ByteOrder::BigEndian : Signal::ByteOrder::LittleEndian;
		nst._default_value = st.default_value;
		nst._factor = st.factor;
		nst._maximum = st.maximum;
		nst._minimum = st.minimum;
		nst._name = st.name;
		nst._offset = st.offset;
		nst._signal_size = st.size;
		nst._unit = st.unit;
		nst._value_type = st.value_type == '+' ? Signal::ValueType::Unsigned : Signal::ValueType::Signed;
	}
	struct VisitorComment
	{
	using variant_comment_t = boost::variant<G_CommentNetwork, G_CommentNode, G_CommentMessage, G_CommentSignal, G_CommentEnvVar>;
		VisitorComment(NetworkImpl& net)
			: _net(net)
		{}
		void operator()(const G_CommentNetwork& cn) const
		{
			_net._comment = cn.comment;
		}
		void operator()(const G_CommentNode& cn) const
		{
			_net._nodes[cn.node_name]._comment = cn.comment;
		}
		void operator()(const G_CommentMessage& cn) const
		{
			_net._messages[cn.message_id]._comment = cn.comment;
		}
		void operator()(const G_CommentSignal& cn) const
		{
			_net._messages[cn.message_id]._signals[cn.signal_name]._comment = cn.comment;
		}
		void operator()(const G_CommentEnvVar& cn) const
		{
			_net._environment_variables[cn.env_var_name]._comment = cn.comment;
		}

	private:
		NetworkImpl& _net;
	};
	for (auto& c : gnet.comments)
	{
		boost::apply_visitor(VisitorComment(net), c);
	}
	struct VisitorValueType
	{
		VisitorValueType(AttributeDefinitionImpl& ad)
			: _ad(ad)
		{}
		void operator()(const G_AttributeValueTypeInt& cn) const
		{
			AttributeDefinition::ValueTypeInt vt;
			vt.minimum = cn.minimum;
			vt.maximum = cn.maximum;
			_ad._value_type = std::move(vt);
		}
		void operator()(const G_AttributeValueTypeHex& cn) const
		{
			AttributeDefinition::ValueTypeHex vt;
			vt.minimum = cn.minimum;
			vt.maximum = cn.maximum;
			_ad._value_type = std::move(vt);
		}
		void operator()(const G_AttributeValueTypeFloat& cn) const
		{
			AttributeDefinition::ValueTypeFloat vt;
			vt.minimum = cn.minimum;
			vt.maximum = cn.maximum;
			_ad._value_type = std::move(vt);
		}
		void operator()(const G_AttributeValueTypeString& cn) const
		{
		}
		void operator()(const G_AttributeValueTypeEnum& cn) const
		{
			AttributeDefinition::ValueTypeEnum vt;
			for (auto& e : cn.values)
			{
				vt.values.emplace_back(e);
			}
			_ad._value_type = std::move(vt);
		}

	private:
		AttributeDefinitionImpl& _ad;
	};
	for (auto& ad : gnet.attribute_definitions)
	{
		AttributeDefinitionImpl& nad = net._attribute_definitions[ad.name];
		nad._name = ad.name;
		if (!ad.object_type)
		{
			nad._object_type = AttributeDefinition::ObjectType::Network;
		}
		else if (*ad.object_type == "BU_")
		{
			nad._object_type = AttributeDefinition::ObjectType::Node;
		}
		else if (*ad.object_type == "BO_")
		{
			nad._object_type = AttributeDefinition::ObjectType::Message;
		}
		else if (*ad.object_type == "SG_")
		{
			nad._object_type = AttributeDefinition::ObjectType::Signal;
		}
		else if (*ad.object_type == "EV_")
		{
			nad._object_type = AttributeDefinition::ObjectType::EnvironmentVariable;
		}
		boost::apply_visitor(VisitorValueType(nad), ad.value_type.value);
	}
	struct VisitorAttribute
	{
		VisitorAttribute(AttributeImpl& a)
			: _a(a)
		{}
		void operator()(uint64_t cn) const
		{
			Attribute::HexValue v;
			v.value = cn;
			_a._value = v;
		}
		void operator()(int64_t cn) const
		{
			Attribute::IntegerValue v;
			v.value = cn;
			_a._value = v;
		}
		void operator()(double cn) const
		{
			Attribute::FloatValue v;
			v.value = cn;
			_a._value = v;
		}
		void operator()(const std::string& cn) const
		{
			Attribute::StringValue v;
			v.value = cn;
			_a._value = v;
		}

	private:
		AttributeImpl& _a;
	};
	for (auto& ad : gnet.attribute_defaults)
	{
		AttributeImpl& nad = net._attribute_defaults[ad.name];
		boost::apply_visitor(VisitorAttribute(nad), ad.value);
	}
	struct VisitorAttributeValues
	{
		VisitorAttributeValues(NetworkImpl& net)
			: _net(net)
		{}
		void operator()(const G_AttributeNetwork& cn) const
		{
			AttributeImpl& attr = _net._attribute_values[cn.attribute_name];
			boost::apply_visitor(VisitorAttribute(attr), cn.value);
			attr._name = cn.attribute_name;
			attr._object_type = AttributeDefinition::ObjectType::Network;
		}
		void operator()(const G_AttributeNode& cn) const
		{
			AttributeImpl& attr = _net._nodes[cn.node_name]._attribute_values[cn.attribute_name];
			boost::apply_visitor(VisitorAttribute(attr), cn.value);
			attr._name = cn.attribute_name;
			attr._object_type = AttributeDefinition::ObjectType::Node;
		}
		void operator()(const G_AttributeMessage& cn) const
		{
			AttributeImpl& attr = _net._messages[cn.message_id]._attribute_values[cn.attribute_name];
			boost::apply_visitor(VisitorAttribute(attr), cn.value);
			attr._name = cn.attribute_name;
			attr._object_type = AttributeDefinition::ObjectType::Message;
		}
		void operator()(const G_AttributeSignal& cn) const
		{
			AttributeImpl& attr = _net._messages[cn.message_id]._signals[cn.signal_name]._attribute_values[cn.attribute_name];
			boost::apply_visitor(VisitorAttribute(attr), cn.value);
			attr._name = cn.attribute_name;
			attr._object_type = AttributeDefinition::ObjectType::Signal;
		}
		void operator()(const G_AttributeEnvVar& cn) const
		{
			AttributeImpl& attr = _net._environment_variables[cn.env_var_name]._attribute_values[cn.attribute_name];
			boost::apply_visitor(VisitorAttribute(attr), cn.value);
			attr._name = cn.attribute_name;
			attr._object_type = AttributeDefinition::ObjectType::EnvironmentVariable;
		}

	private:
		NetworkImpl& _net;
	};
	for (auto& av : gnet.attribute_values)
	{
		boost::apply_visitor(VisitorAttributeValues(net), av);
	}
}
