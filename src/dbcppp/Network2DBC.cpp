
#include "../../include/dbcppp/Network2DBC.h"
#include "NetworkImpl.h"

using namespace dbcppp;
using namespace dbcppp::Network2DBC;

DBCPPP_API std::ostream& dbcppp::Network2DBC::operator<<(std::ostream& os, const na_t& na)
{
    const auto& net = std::get<0>(na);
    const auto& a = std::get<1>(na);
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
    if (net.getAttributeDefaultByName(a.getName()) == &a)
    {
        cmd = "BA_DEF_DEF_";
    }
    os << cmd << " \"" << a.getName() << "\"";
    switch (a.getObjectType())
    {
    case AttributeDefinition::ObjectType::Network:
    {
        boost::apply_visitor(Visitor(os), a.getValue());
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
                                return &av == &a;
                            });
                        return av != nullptr;
                    });
                return n ? n->getName() : "";
            };
        os << " BU_ " << find_node_name();
        boost::apply_visitor(Visitor(os), a.getValue());
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
                                return &av == &a;
                            });
                        return av != nullptr;
                    });
                return m ? m->getId() : uint64_t(-1);
            };
        os << " BO_ " << find_message_id();
        boost::apply_visitor(Visitor(os), a.getValue());
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
                                        return &av == &a;
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
        boost::apply_visitor(Visitor(os), a.getValue());
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
                                return &av == &a;
                            });
                        return av != nullptr;
                    });
                return ev ? ev->getName() : "";

            };
        os << " EV_ " << find_environment_variable_name();
        boost::apply_visitor(Visitor(os), a.getValue());
        break;
    }
    }
    os << ";";
    return os;
}
DBCPPP_API std::ostream& dbcppp::Network2DBC::operator<<(std::ostream& os, const AttributeDefinition& ad)
{
    struct VisitorValueType
    {
        VisitorValueType(std::ostream& os)
            : _os(os)
        {}
        void operator()(const AttributeDefinition::ValueTypeInt& vt) const
        {
            _os << " INT" << " " << vt.minimum << " " << vt.maximum;
        }
        void operator()(const AttributeDefinition::ValueTypeHex& vt) const
        {
            _os << " HEX" << " " << vt.minimum << " " << vt.maximum;
        }
        void operator()(const AttributeDefinition::ValueTypeFloat& vt) const
        {
            _os << " FLOAT" << " " << vt.minimum << " " << vt.maximum;
        }
        void operator()(const AttributeDefinition::ValueTypeString& vt) const
        {
            _os << " STRING";
        }
        void operator()(const AttributeDefinition::ValueTypeEnum& vt) const
        {
            _os << " ENUM";
            const auto& values = vt.values;
            if (values.size())
            {
                auto iter = values.begin();
                _os << " \"" << *iter << "\"";
                for (iter++; iter != values.end(); iter++)
                {
                    _os << ", \"" << *iter << "\"";
                }
            }
        }

    private:
        std::ostream& _os;
    };

    std::string object_type = "";
    switch (ad.getObjectType())
    {
    case AttributeDefinition::ObjectType::Network: break;
    case AttributeDefinition::ObjectType::Node: object_type = "BU_"; break;
    case AttributeDefinition::ObjectType::Message: object_type = "BO_"; break;
    case AttributeDefinition::ObjectType::Signal: object_type = "SG_"; break;
    case AttributeDefinition::ObjectType::EnvironmentVariable: object_type = "EV_"; break;
    }
    os << "BA_DEF_ ";
    if (object_type != "")
    {
        os << object_type << " ";
    }
    os << "\"" << ad.getName() << "\"";
    boost::apply_visitor(VisitorValueType(os), ad.getValueType());
    os << ";";
    return os;
}
DBCPPP_API std::ostream& dbcppp::Network2DBC::operator<<(std::ostream& os, const BitTiming& bt)
{
    os << "BS_:";
    if (bt.getBaudrate() != 0 && bt.getBTR1() != 0 && bt.getBTR2() != 0)
    {
        os << " " << bt.getBaudrate() << " : " << bt.getBTR1() << ", " << bt.getBTR2();
    }
    return os;
}
DBCPPP_API std::ostream& dbcppp::Network2DBC::operator<<(std::ostream& os, const EnvironmentVariable& ev)
{
    os << "EV_ " << ev.getName() << ": ";
    switch (ev.getVarType())
    {
    case EnvironmentVariable::VarType::Integer: os << "0"; break;
    case EnvironmentVariable::VarType::Float: os << "1"; break;
    case EnvironmentVariable::VarType::String: os << "2"; break;
    case EnvironmentVariable::VarType::Data: os << "0"; break;
    }
    os << " [" << ev.getMinimum() << "|" << ev.getMaximum() << "]" << " \"" << ev.getUnit() << "\" "
        << ev.getInitialValue() << " " << ev.getEvId() << " ";
    switch (ev.getAccessType())
    {
    case EnvironmentVariable::AccessType::Unrestricted: os << "DUMMY_NODE_VECTOR0"; break;
    case EnvironmentVariable::AccessType::Read: os << "DUMMY_NODE_VECTOR1"; break;
    case EnvironmentVariable::AccessType::Write: os << "DUMMY_NODE_VECTOR2"; break;
    case EnvironmentVariable::AccessType::ReadWrite: os << "DUMMY_NODE_VECTOR3"; break;
    }
    bool first = true;
    ev.forEachAccessNode(
        [&](const std::string& n)
        {
            if (first)
            {
                os << " " << n;
                first = false;
            }
            else
            {
                os << ", " << n;
            }
        });
    os << ";";
    return os;
}
DBCPPP_API std::ostream& dbcppp::Network2DBC::operator<<(std::ostream& os, const Message& m)
{
    os << "BO_ " << m.getId() << " " << m.getName() << ": " << m.getMessageSize() << " " << m.getTransmitter();
    m.forEachSignal(
        [&](const Signal& s)
        {
            os << "\n ";
            os << s;
        });
    return os;
}
DBCPPP_API std::ostream& dbcppp::Network2DBC::operator<<(std::ostream& os, const Network& net)
{
    os << "VERSION \"";
    if (net.getVersion() != "")
    {
        os << net.getVersion();
    }
    os << "\"";
    os << "\n";
    os << "NS_:";
    net.forEachNewSymbol(
        [&](const std::string& ns)
        {
            os << "\n " << ns;
        });
    os << "\n";
    os << net.getBitTiming();
    os << "\n";
    os << "BU_:";
    net.forEachNode(
        [&](const Node& n)
        {
            os << " " << n.getName(); 
        });
    net.forEachValueTable(
        [&](const ValueTable& vt)
        {
            os << "\n";
            os << vt;
        });
    net.forEachMessage(
        [&](const Message& m)
        {
            os << "\n";
            os << m;
        });
    // serialize message_transmitters
    net.forEachMessage(
        [&](const Message& m)
        {
            bool first = true;
            m.forEachMessageTransmitter(
                [&](const std::string& n)
                {
                    if (first)
                    {
                        first = false;
                        os << "\n";
                        os << "BO_TX_BU_ " << m.getId() << " :";
                        os << " " << n;
                    }
                    else
                    {
                        os << ", " << n;
                    }
                });
            if (!first)
            {
                os << ";";
            }
        });
    net.forEachEnvironmentVariable(
        [&](const EnvironmentVariable& ev)
        {
            os << "\n";
            os << ev;
        });
    net.forEachEnvironmentVariable(
        [&](const EnvironmentVariable& ev)
        {
            if (ev.getVarType() == EnvironmentVariable::VarType::Data)
            {
                os << "\n";
                os << "ENVVAR_DATA_ " << ev.getName() << " : " << ev.getDataSize() << ";";
            }
        });
    net.forEachValueTable(
        [&](const ValueTable& vt)
        {
            if (vt.getSignalType())
            {
                os << "\n";
                os << vt;
            }
        });
    // serialize comments
    // Network comment
    if (net.getComment() != "")
    {
        os << "\n";
        os << "CM_ \"" << net.getComment() << "\";";
    }
    // Node comments
    net.forEachNode(
        [&](const Node& n)
        {
            if (n.getComment() != "")
            {
                os << "\n";
                os << "CM_ BU_ " << n.getName() << " \"" << n.getComment() << "\"" << ";";
            }
        });
    // Message comments
    net.forEachMessage(
        [&](const Message& m)
        {
            if (m.getComment() != "")
            {
                os << "\n";
                os << "CM_ BO_ " << m.getId() << " \"" << m.getComment() << "\"" << ";";
            }
        });
    // Signal comments
    net.forEachMessage(
        [&](const Message& m)
        {
            m.forEachSignal(
                [&](const Signal& s)
                {
                    if (s.getComment() != "")
                    {
                        os << "\n";
                        os << "CM_ SG_ " << m.getId() << " " << s.getName() << " \"" << s.getComment() << "\"" << ";";
                    }
                });
        });
    // EnvironmentVariable comments
    net.forEachEnvironmentVariable(
        [&](const EnvironmentVariable& ev)
        {
            if (ev.getComment() != "")
            {
                os << "\n";
                os << "CM_ EV_ " << ev.getName() << " \"" << ev.getComment() << "\"" << ";";
            }
        });
    net.forEachAttributeDefinition(
        [&](const AttributeDefinition& ad)
        {
            os << "\n";
            os << ad;
        });
    net.forEachAttributeDefault(
        [&](const Attribute& ad)
        {
            os << "\n";
            os << na_t{net, ad};
        });
    net.forEachAttributeValue(
        [&](const Attribute& av)
        {
            os << "\n";
            os << na_t{net, av};
        });
    net.forEachNode(
        [&](const Node& n)
        {
            n.forEachAttributeValue(
                [&](const Attribute& av)
                {
                    os << "\n";
                    os << na_t{net, av};
                });
        });
    net.forEachMessage(
        [&](const Message& m)
        {
            m.forEachAttributeValue(
                [&](const Attribute& av)
                {
                    os << "\n";
                    os << na_t{net, av};
                });
        });
    net.forEachMessage(
        [&](const Message& m)
        {
            m.forEachSignal(
                [&](const Signal& s)
                {
                    s.forEachAttributeValue(
                        [&](const Attribute& av)
                        {
                            os << "\n";
                            os << na_t{net, av};
                        });
                });
        });
    net.forEachEnvironmentVariable(
        [&](const EnvironmentVariable& ev)
        {
            ev.forEachAttributeValue(
                [&](const Attribute& av)
                {
                    os << "\n";
                    os << na_t{net, av};
                });
        });
    // Serialize value descriptions
    net.forEachMessage(
        [&](const Message& m)
        {
            m.forEachSignal(
                [&](const Signal& s)
                {
                    bool first = true;
                    s.forEachValueDescription(
                        [&](double value, const std::string& desc)
                        {
                            if (first)
                            {
                                first = false;
                                os << "\n";
                                os << "VAL_ " << m.getId() << " " << s.getName();
                            }
                            os << " " << value << " \"" << desc << "\"";
                        });
                    if (!first)
                    {
                        os << ";";
                    }
                });
        });
    net.forEachEnvironmentVariable(
        [&](const EnvironmentVariable& ev)
        {
            bool first = true;
            ev.forEachValueDescription(
                [&](double value, const std::string& desc)
                {
                    if (first)
                    {
                        first = false;
                        os << "\n";
                        os << "VAL_ " << ev.getName();
                    }
                    os << " " << value << " \"" << desc << "\"";
                });
            if (!first)
            {
                os << ";";
            }
        });
    net.forEachMessage(
        [&](const Message& m)
        {
            m.forEachSignal(
                [&](const Signal& s)
                {
                    if (s.getExtendedValueType() != Signal::ExtendedValueType::Integer)
                    {
                        uint64_t type = 0;
                        switch (s.getExtendedValueType())
                        {
                        case Signal::ExtendedValueType::Float: type = 1; break;
                        case Signal::ExtendedValueType::Double: type = 2; break;
                        }
                        os << "\n";
                        os << "SIG_VALTYPE_ " << m.getId() << " " << s.getName() << " : " << type << ";";
                    }
                });
        });
    return os;
}
DBCPPP_API std::ostream& dbcppp::Network2DBC::operator<<(std::ostream& os, const Node& n)
{
    os << n.getName();
    return os;
}
DBCPPP_API std::ostream& dbcppp::Network2DBC::operator<<(std::ostream& os, const Signal& s)
{
    os << "SG_ " << s.getName() << " ";
    switch (s.getMultiplexerIndicator())
    {
    case Signal::Multiplexer::MuxSwitch: os << "M "; break;
    case Signal::Multiplexer::MuxValue: os << "m" << s.getMultiplexerSwitchValue() << " "; break;
    }
    os << ": " << s.getStartBit() << "|" << s.getBitSize() << "@";
    switch (s.getByteOrder())
    {
    case Signal::ByteOrder::BigEndian: os << "0"; break;
    case Signal::ByteOrder::LittleEndian: os << "1"; break;
    }
    switch (s.getValueType())
    {
    case Signal::ValueType::Unsigned: os << "+ "; break;
    case Signal::ValueType::Signed: os << "- "; break;
    }
    os << "(" << s.getFactor() << "," << s.getOffset() << ") ";
    os << "[" << s.getMinimum() << "|" << s.getMaximum() << "] ";
    os << "\"" << s.getUnit() << "\"";
    s.forEachReceiver(
        [&](const std::string& n)
        {
            os << " " << n;
        });
    return os;
}
DBCPPP_API std::ostream& dbcppp::Network2DBC::operator<<(std::ostream& os, const SignalType& st)
{
    os << "SGTYPE_ " << st.getName() << " : " << st.getSignalSize() << "@";
    switch (st.getByteOrder())
    {
    case Signal::ByteOrder::BigEndian: os << "0"; break;
    case Signal::ByteOrder::LittleEndian: os << "1"; break;
    }
    switch (st.getValueType())
    {
    case Signal::ValueType::Unsigned: os << "+ "; break;
    case Signal::ValueType::Signed: os << "- "; break;
    }
    os << "(" << st.getFactor() << "," << st.getOffset() << ") ";
    os << "[" << st.getMinimum() << "|" << st.getMaximum() << "] ";
    os << "\"" << st.getUnit() << "\" " << st.getDefaultValue();
    os << ", " << st.getValueTable();
    os << ";";
    return os;
}
DBCPPP_API std::ostream& dbcppp::Network2DBC::operator<<(std::ostream& os, const ValueTable& vt)
{
    bool first = true;
    vt.forEachValueEncodingDescription(
        [&](double value, const std::string& desc)
        {
            if (first)
            {
                first = false;
                os << "VAL_TABLE_ " << vt.getName();
            }
            os << " " << value << " \"" << desc << "\"";
        });
    if (!first)
    {
        os << ";";
    }
    return os;
}
