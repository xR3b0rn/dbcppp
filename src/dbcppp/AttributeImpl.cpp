
#include <boost/variant.hpp>
#include "AttributeImpl.h"
#include "../../include/dbcppp/Network.h"

using namespace dbcppp;

std::unique_ptr<Attribute> Attribute::create(
      std::string&& name
    , AttributeDefinition::ObjectType object_type
    , value_t value)
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
std::unique_ptr<Attribute> AttributeImpl::clone() const
{
    return std::make_unique<AttributeImpl>(*this);
}
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
    if (net.getAttributeDefaultByName(this->getName()) == this)
    {
        cmd = "BA_DEF_DEF_";
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
                const Node* n = net.findNode(
                    [&](const Node& n)
                    {
                        const Attribute* av = n.findAttributeValue(
                            [&](const Attribute& av)
                            {
                                return &av == this;
                            });
                        return av != nullptr;
                    });
                return n ? n->getName() : "";
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
                const Message* m = net.findMessage(
                    [&](const Message& m)
                    {
                        const Attribute* av = m.findAttributeValue(
                            [&](const Attribute& av)
                            {
                                return &av == this;
                            });
                        return av != nullptr;
                    });
                return m ? m->getId() : uint64_t(-1);
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
                const Signal* sig = nullptr;
                const Message* m = net.findMessage(
                    [&](const Message& m)
                    {
                        const Signal* s = m.findSignal(
                            [&](const Signal& s)
                            {
                                const Attribute* av = s.findAttributeValue(
                                    [&](const Attribute& av)
                                    {
                                        return &av == this;
                                    });
                                if (av != nullptr)
                                {
                                    sig = &s;
                                }
                                return av != nullptr;
                            });
                        return s != nullptr;
                    });
                return sig;
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
                const EnvironmentVariable* ev = net.findEnvironmentVariable(
                    [&](const EnvironmentVariable& ev)
                    {
                        const Attribute* av = ev.findAttributeValue(
                            [&](const Attribute& av)
                            {
                                return &av == this;
                            });
                        return av != nullptr;
                    });
                return ev ? ev->getName() : "";

            };
        os << " EV_ " << find_environment_variable_name();
        boost::apply_visitor(Visitor(os), getValue());
        break;
    }
    }
    os << ";";
}

