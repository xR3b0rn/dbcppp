
#include <boost/variant.hpp>
#include "AttributeImpl.h"
#include "Signal.h"
#include "Network.h"

using namespace dbcppp;

std::unique_ptr<Attribute> Attribute::create(
	  std::string&& name
	, AttributeDefinition::ObjectType object_type
	, value_t&& value)
{
	return std::make_unique<AttributeImpl>(
		  std::move(name)
		, object_type
		, std::move(value));
}

AttributeImpl::AttributeImpl(std::string&& name, AttributeDefinition::ObjectType object_type, Attribute::value_t value)
	: _name(std::move(name))
	, _object_type(std::move(object_type))
	, _value(std::move(value))
{}
const std::string& AttributeImpl::getName() const
{
	return _name;
}
AttributeDefinition::ObjectType AttributeImpl::getObjectType() const
{
	return _object_type;
};
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
		void operator()(int64_t i) const
		{
			_os << " " << i;
		}
		void operator()(double d) const
		{
			_os << " " << d;
		}
		void operator()(const std::string& s) const
		{
			_os << " \"" << s << "\"";
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
		auto find_message_id =
			[&]()
			{
				for (auto m : net.getMessages())
				{
					auto values = m.second->getAttributeValues();
					auto iter =
						std::find_if(values.begin(), values.end(),
							[&](const auto& p)
							{
								return p.second == this;
							});
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

