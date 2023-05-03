#ifdef ENABLE_KCD
#include <map>
#include <memory>
#include <vector>
#include <optional>
#include <sstream>
#include <cstring>

#include <libxmlmm.h>

#include "dbcppp/Network.h"

using namespace dbcppp;

class KCDParserError
    : public std::runtime_error
{
public:
    using base1_t = std::runtime_error;

    KCDParserError(const std::string& msg)
        : base1_t("libdbcppp: " + msg)
    {}
};

class KCD
{
public:
    struct Value
    {
        ISignal::EValueType value_type{ISignal::EValueType::Unsigned};
        ISignal::EExtendedValueType extended_value_type{ISignal::EExtendedValueType::Integer};
        double factor{1.};
        double offset{0.};
        std::string unit{""};
        double min{0.};
        double max{1.};
    };

    std::map<std::string, std::unique_ptr<INetwork>> _networks;

    KCD(std::istream& is)
    {
        xml::Document doc;
        doc.read_from_stream(is);
        parseNetworkDefinition(doc.get_root_element());
        auto nodes = parseNodes(doc.get_root_element());
        _networks = std::move(parseNetworks(doc.get_root_element(), nodes));
    }
    void
        parseNetworkDefinition(const xml::Element* net_def)
    {
        if (net_def->get_name() != "NetworkDefinition")
        {
            throw KCDParserError("could not find root node \"NetworkDefinition\"");
        }
    }
    std::vector<std::unique_ptr<INode>>
        parseNodes(const xml::Element* net_def)
    {
        std::vector<std::unique_ptr<INode>> result;
        const auto nodes = net_def->find_elements("/*[local-name() = 'NetworkDefinition']/*[local-name() = 'Node']");
        for (const auto* node : nodes)
        {
            auto n = parseNode(node);
            result.push_back(std::move(n));
        }
        return std::move(result);
    }
    std::unique_ptr<INode>
        parseNode(const xml::Element* node)
    {
        auto name = std::string();
        if (node->has_attribute("name"))
        {
            name = node->get_attribute("name");
        }
        if (!node->has_attribute("id"))
        {
            throw KCDParserError("");
        }
        auto id = node->get_attribute<std::size_t>("id");
        _node_id_to_node_name[id] = name;
        return INode::Create(std::move(name), "", {});
    }
    std::unique_ptr<IAttribute>
        parseAttribute(const xml::Element* var, IAttributeDefinition::EObjectType object_type)
    {
        std::string name = var->get_attribute("name");
        IAttribute::value_t value = var->get_attribute("Value");
        std::stringstream ss;
        std::visit([&](auto v) { ss << v; }, value);
        {
            int64_t i64value;
            ss >> i64value;
            if (!ss.eof())
            {
                double dvalue;
                ss >> dvalue;
                if (ss.eof())
                {
                    value = dvalue;
                }
            }
            else
            {
                value = i64value;
            }
        }
        return IAttribute::Create(std::move(name), object_type, value);
    }
    std::map<std::string, std::unique_ptr<INetwork>>
        parseNetworks(const xml::Element* net_def, const std::vector<std::unique_ptr<INode>>& nodes)
    {
        std::map<std::string, std::unique_ptr<INetwork>> networks;
        const auto buses = net_def->find_elements("/*[local-name() = 'NetworkDefinition']/*[local-name() = 'Bus']");
        for (const auto* bus : buses)
        {
            networks.insert(parseNetwork(bus, nodes));
        }
        return std::move(networks);
    }
    std::pair<std::string, std::unique_ptr<INetwork>>
        parseNetwork(const xml::Element* bus, const std::vector<std::unique_ptr<INode>>& nodes)
    {
        auto name = bus->get_attribute("name");
        auto bit_timing = parseBitTiming(bus);
        auto msgs = std::vector<std::unique_ptr<IMessage>>();
        const auto messages = bus->find_elements("./*[local-name() = 'Message']");
        for (const auto* message : messages)
        {
            auto msg = parseMessage(message);
            msgs.push_back(std::move(msg));
        }
        auto ns = std::vector<std::unique_ptr<INode>>();
        for (const auto& node : nodes)
        {
            ns.push_back(node->Clone());
        }
        return std::make_pair(
              name
            , INetwork::Create(
                  ""
                , {}
                , std::move(bit_timing)
                , std::move(ns)
                , {}
                , std::move(msgs)
                , {}, {}, {}, {}, ""));
    }
    std::unique_ptr<IBitTiming>
        parseBitTiming(const xml::Element* bus)
    {
        uint64_t baudrate = 500000;
        if (bus->has_attribute("baudrate"))
        {
            baudrate = bus->get_attribute<uint64_t>("baudrate");
        }
        return IBitTiming::Create(baudrate, 0, 0);
    }
    uint64_t
        getMessageSize(const xml::Element* message)
    {
        uint64_t max = 0;
        const auto signals = message->find_elements("./*[local-name() = 'Signal']");
        for (const auto* signal : signals)
        {
            uint64_t offset = signal->get_attribute<uint64_t>("offset");
            uint64_t length = 1;
            if (signal->has_attribute("length"))
            {
                length = signal->get_attribute<uint64_t>("length");
            }
            if (max < offset + length)
            {
                max = offset + length;
            }
        }
        return (max + 7) / 8;
    }
    std::unique_ptr<IMessage>
        parseMessage(const xml::Element* message)
    {
        auto str_id = message->get_attribute("id");
        auto id = uint64_t();
        if (str_id[1] == 'x' || str_id[1] == 'X')
        {
            std::stringstream ss;
            ss << std::hex << str_id;
            ss >> id;
        }
        else
        {
            std::stringstream ss;
            ss << str_id;
            ss >> id;
        }
        auto name = message->get_attribute("name");
        auto comment = parseComment(message);
        const auto* node_ref = message->find_element("./*[local-name() = 'Producer']/*[local-name() = 'NodeRef']");
        auto producer = _node_id_to_node_name.find(node_ref->get_attribute<uint64_t>("id"))->second;
        auto message_size = uint64_t(0);
        if (!message->has_attribute("length") ||
            message->get_attribute("length") == "auto")
        {
            message_size = getMessageSize(message);
        }
        else
        {
            message_size = message->get_attribute<uint64_t>("length");
        }
        const auto* note = message->find_element("./*[local-name() = 'Notes']");
        auto nt = parseComment(note);
        auto transmitter = parseTransmitter(message);
        auto signals = parseSignals(message, message_size);
        return IMessage::Create(
              id
            , std::move(name)
            , message_size
            , std::move(producer)
            , std::move(transmitter)
            , std::move(signals)
            , {}
            , std::move(comment)
            , {});
    }
    std::string
        parseComment(const xml::Element* note)
    {
        std::string result;
        if (note != nullptr)
        {
            result = note->get_text();
        }
        return result;
    }
    std::vector<std::string>
        parseTransmitter(const xml::Element* message)
    {
        std::vector<std::string> result;
        const auto node_refs = message->find_elements("./*[local-name() = 'Producer']/*[local-name() = 'NodeRef']");
        for (const auto* node_ref : node_refs)
        {
            auto id = node_ref->get_attribute<std::size_t>("id");
            const auto node_name = _node_id_to_node_name.find(id);
            if (node_name == _node_id_to_node_name.end())
            {
                throw KCDParserError("could not find node with ID \"" + std::to_string(id) + "\"");
            }
            result.push_back(node_name->second);
        }
        return result;
    }
    std::vector<std::unique_ptr<ISignal>>
        parseSignals(const xml::Element* message, uint64_t message_size)
    {
        std::vector<std::unique_ptr<ISignal>> sigs;
        const auto* multiplex = message->find_element("./*[local-name() = 'Multiplex']");
        if (multiplex != nullptr)
        {
            auto mux_signal = parseSignal(multiplex, message_size, ISignal::EMultiplexer::MuxSwitch, 0);
            sigs.push_back(std::move(mux_signal));
            const auto mux_groups = multiplex->find_elements("./*[local-name() = 'MuxGroup']");
            for (const auto* mux_group : mux_groups)
            {
                uint64_t multiplex_indicator = mux_group->get_attribute<uint64_t>("count");
                const auto signals = mux_group->find_elements("./*[local-name() = 'Signal']");
                for (const auto* signal : signals)
                {
                    auto sig = parseSignal(signal, message_size, ISignal::EMultiplexer::MuxValue, multiplex_indicator);
                    sigs.push_back(std::move(sig));
                }
            }
        }
        const auto signals = message->find_elements("./*[local-name() = 'Signal']");
        for (const auto* signal : signals)
        {
            auto sig = parseSignal(signal, message_size, ISignal::EMultiplexer::NoMux, 0);
            sigs.push_back(std::move(sig));
        }
        return std::move(sigs);
    }
    std::unique_ptr<ISignal>
        parseSignal(const xml::Element* signal, uint64_t message_size, ISignal::EMultiplexer mux, uint64_t multiplex_indicator)
    {
        auto comment = parseComment(signal->find_element("Notes"));
        auto receivers = parseReceivers(signal);
        auto name = signal->get_attribute("name");
        auto start_bit = signal->get_attribute<uint64_t>("offset");
        auto bit_size = uint64_t(1);
        if (signal->has_attribute("length"))
        {
            bit_size = signal->get_attribute<uint64_t>("length");
        }
        auto byte_order = ISignal::EByteOrder::LittleEndian;
        auto value_type = ISignal::EValueType::Unsigned;
        auto extended_value_type = ISignal::EExtendedValueType::Integer;
        auto factor = 1.;
        auto offset = 0.;
        auto unit = std::string("");
        auto min = 0.;
        auto max = 1.;
        auto value_encoding_descriptions = std::vector<std::unique_ptr<IValueEncodingDescription>>();
        if (signal->has_attribute("endianess") &&
            signal->get_attribute("endianess") == "big")
        {
            byte_order = ISignal::EByteOrder::BigEndian;
        }
        const auto* value = signal->find_element("./*[local-name() = 'Value']");
        if (value != nullptr)
        {
            auto v = parseValue(value);
            value_type = v.value_type;
            extended_value_type = v.extended_value_type;
            factor = v.factor;
            offset = v.offset;
            unit = v.unit;
            min = v.min;
            max = v.max;
        }
        // we are ignoring LabelGroup, it is not supported by the library
        const auto label_set = signal->find_elements("./*[local-name() = 'LabelSet']/*[local-name() = 'Label']");
        for (const auto* label : label_set)
        {
            auto value = label->get_attribute<int64_t>("value");
            auto name = label->get_attribute("name");
            auto ved = IValueEncodingDescription::Create(value, std::move(name));
            value_encoding_descriptions.push_back(std::move(ved));
        }
        return ISignal::Create(
              message_size
            , std::move(name)
            , mux
            , multiplex_indicator
            , start_bit
            , bit_size
            , byte_order
            , value_type
            , factor
            , offset
            , min
            , max
            , std::move(unit)
            , std::move(receivers)
            , {}
            , std::move(value_encoding_descriptions)
            , std::move(comment)
            , extended_value_type
            , {});
    }
    std::vector<std::string>
        parseReceivers(const xml::Element* signal)
    {
        std::vector<std::string> result;
        const auto node_refs = signal->find_elements("./*[local-name() = 'Consumer']/*[local-name() = 'NodeRef']");
        for (const auto* node_ref : node_refs)
        {
            auto id = node_ref->get_attribute<std::size_t>("id");
            const auto node_name = _node_id_to_node_name.find(id);
            result.push_back(node_name->second);
        }
        return result;
    }
    Value
        parseValue(const xml::Element* value)
    {
        Value result;
        if (value->has_attribute("type"))
        {
            auto vt = value->get_attribute("type");
            if (vt == "signed")
            {
                result.value_type = ISignal::EValueType::Signed;
            }
            else if (vt == "single")
            {
                result.extended_value_type = ISignal::EExtendedValueType::Float;
            }
            else if (vt == "double")
            {
                result.extended_value_type = ISignal::EExtendedValueType::Double;
            }
        }
        if (value->has_attribute("slope"))
        {
            result.factor = value->get_attribute<double>("slope");
        }
        if (value->has_attribute("intercept"))
        {
            result.offset = value->get_attribute<double>("intercept");
        }
        if (value->has_attribute("unit"))
        {
            result.unit = value->get_attribute("unit");
        }
        if (value->has_attribute("min"))
        {
            result.min = value->get_attribute<double>("min");
        }
        if (value->has_attribute("max"))
        {
            result.min = value->get_attribute<double>("max");
        }
        return result;
    }

private:
    std::map<std::size_t, std::string> _node_id_to_node_name;
};

std::map<std::string, std::unique_ptr<INetwork>> INetwork::LoadKCDFromIs(std::istream& is)
{
    KCD kcd(is);
    return std::move(kcd._networks);
}
#endif
