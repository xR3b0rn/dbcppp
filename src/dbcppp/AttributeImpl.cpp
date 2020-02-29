
#include <boost/variant.hpp>
#include "AttributeImpl.h"
#include "Signal.h"
#include "Network.h"

using namespace dbcppp;

const std::string& AttributeImpl::getName() const
{
	return _name;
}
AttributeDefinition::ObjectType AttributeImpl::getObjectType() const
{
	return _object_type;
}
const Attribute::value_t& AttributeImpl::getValue() const
{
	return _value;
}

void Attribute::serializeToStream(std::ostream& os, const Network& net) const
{
	struct Visitor
	{
		Visitor(std::ostream& os)
			: _os(os)
		{}
		void operator()(const Attribute::IntegerValue& i) const
		{
			_os << " " << i.value;
		}
		void operator()(const Attribute::HexValue& h) const
		{
			_os << " " << h.value;
		}
		void operator()(const Attribute::FloatValue& f) const
		{
			_os << " " << f.value;
		}
		void operator()(const Attribute::EnumValue& e) const
		{
			_os << " " << e.value;
		}
		void operator()(const Attribute::StringValue& s) const
		{
			_os << " \"" << s.value << "\"";
		}

		std::ostream& _os;
	};

	std::string cmd = "BA_";
	for (const auto& ad : net.getAttributeDefaults())
	{
		if (ad.second == this)
		{
			cmd = "BA_DEF_DEF_";
			break;
		}
	}
	os << cmd << " \"" << getName() << "\"";
	switch (getObjectType())
	{
	case AttributeDefinition::ObjectType::Network:
	{
		boost::apply_visitor(Visitor(os), getValue());
		break;
	}
	case AttributeDefinition::ObjectType::Node:
	{
		auto find_node_name =
			[&]()
			{
				for (auto n : net.getNodes())
				{
					auto values = n.second->getAttributeValues();
					auto iter =
						std::find_if(values.begin(), values.end(),
							[&](const auto& p)
							{
								return p.second == this;
							});
					if (iter != values.end())
					{
						return n.second->getName();
					}
				}
				return std::string("");
			};
		os << " BU_ " << find_node_name();
		boost::apply_visitor(Visitor(os), getValue());
		break;
	}
	case AttributeDefinition::ObjectType::Message:
	{
		auto pthis = this;
		auto find_message_id =
			[&, pthis]()
			{
				for (auto m : net.getMessages())
				{
					auto values = m.second->getAttributeValues();
					auto iter =
						std::find_if(values.begin(), values.end(),
							[&, pthis](const auto& p)
							{
								return p.second == pthis;
							});
					for (auto iter = values.begin(); iter != values.end(); iter++)
					{
						uint64_t val1 = reinterpret_cast<uint64_t>(pthis);
						uint64_t val2 = reinterpret_cast<uint64_t>(iter->second);
						if (iter->second == pthis)
						{
							return m.second->getId();
						}
					}
					if (iter != values.end())
					{
						return m.second->getId();
					}
				}
				return uint64_t(-1);
			};
		os << " BO_ " << find_message_id();
		boost::apply_visitor(Visitor(os), getValue());
		break;
	}
	case AttributeDefinition::ObjectType::Signal:
	{
		auto find_signal =
			[&]() -> const Signal*
			{
				for (auto m : net.getMessages())
				{
					for (auto s : m.second->getSignals())
					{
						auto values = s.second->getAttributeValues();
						auto iter =
							std::find_if(values.begin(), values.end(),
								[&](const auto& p)
								{
									return p.second == this;
								});
						if (iter != values.end())
						{
							return s.second;
						}
					}
				}
				return nullptr;
			};
		const Signal* sig = find_signal();
		os << " SG_ " << net.findParentMessage(sig)->getId();
		os << " " << sig->getName();
		boost::apply_visitor(Visitor(os), getValue());
		break;
	}
	case AttributeDefinition::ObjectType::EnvironmentVariable:
	{
		auto find_environment_variable_name =
			[&]()
			{
				for (auto ev : net.getEnvironmentVariables())
				{
					auto values = ev.second->getAttributeValues();
					auto iter =
						std::find_if(values.begin(), values.end(),
							[&](const auto& p)
							{
								return p.second == this;
							});
					if (iter != values.end())
					{
						return ev.second->getName();
					}
				}
				return std::string("");
			};
		os << " EV_ " << find_environment_variable_name();
		boost::apply_visitor(Visitor(os), getValue());
		break;
	}
	}
	os << ";";
}

