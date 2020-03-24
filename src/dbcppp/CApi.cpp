
#include "../../include/dbcppp/CApi.h"
#include "NetworkImpl.h"

using namespace dbcppp;

extern "C"
{
    DBCPPP_API dbcppp_Network* dbcppp_NetworkCreate(
          const char* version
        , const char** new_symbols
        , dbcppp_BitTiming* bit_timing
        , dbcppp_Node** nodes
        , dbcppp_ValueTable** value_tables
        , dbcppp_Message** messages
        , dbcppp_EnvironmentVariable** environment_variables
        , dbcppp_AttributeDefinition** attribute_definitions
        , dbcppp_Attribute** attribute_defaults
        , dbcppp_Attribute** attribute_values
        , const char* comment)
    {
        std::string v(version);
        std::set<std::string> ns;
        while (*new_symbols)
        {
            ns.insert(*new_symbols);
            new_symbols++;
        }
        std::unique_ptr<BitTiming> bt(reinterpret_cast<BitTimingImpl*>(bit_timing));
        std::map<std::string, std::unique_ptr<Node>> n;
        while (*nodes)
        {
            NodeImpl* ni = reinterpret_cast<NodeImpl*>(*nodes);
            n.insert(std::make_pair(ni->getName(), std::unique_ptr<Node>(ni)));
            nodes++;
        }
        std::map<std::string, std::unique_ptr<ValueTable>> vt;
        while (*value_tables)
        {
            ValueTableImpl* vti = reinterpret_cast<ValueTableImpl*>(*value_tables);
            vt.insert(std::make_pair(vti->getName(), std::unique_ptr<ValueTable>(vti)));
            value_tables++;
        }
        std::unordered_map<uint64_t, std::unique_ptr<Message>> m;
        while (*messages)
        {
            MessageImpl* mi = reinterpret_cast<MessageImpl*>(*messages);
            m.insert(std::make_pair(mi->getId(), std::unique_ptr<Message>(mi)));
            messages++;
        }
        std::map<std::string, std::unique_ptr<EnvironmentVariable>> ev;
        while (*environment_variables)
        {
            EnvironmentVariableImpl* evi = reinterpret_cast<EnvironmentVariableImpl*>(*environment_variables);
            ev.insert(std::make_pair(evi->getName(), std::unique_ptr<EnvironmentVariable>(evi)));
            environment_variables++;
        }
        std::map<std::string, std::unique_ptr<AttributeDefinition>> adef;
        while (*attribute_definitions)
        {
            AttributeDefinitionImpl* adefi = reinterpret_cast<AttributeDefinitionImpl*>(*attribute_definitions);
            adef.insert(std::make_pair(adefi->getName(), std::unique_ptr<AttributeDefinition>(adefi)));
            attribute_definitions++;
        }
        std::map<std::string, std::unique_ptr<Attribute>> ad;
        while (*attribute_defaults)
        {
            AttributeImpl* adi = reinterpret_cast<AttributeImpl*>(*attribute_defaults);
            ad.insert(std::make_pair(adi->getName(), std::unique_ptr<Attribute>(adi)));
            attribute_defaults++;
        }
        std::map<std::string, std::unique_ptr<Attribute>> av;
        while (*attribute_values)
        {
            AttributeImpl* avi = reinterpret_cast<AttributeImpl*>(*attribute_values);
            ad.insert(std::make_pair(avi->getName(), std::unique_ptr<Attribute>(avi)));
            attribute_values++;
        }
        std::string c(comment);
        auto net = Network::create(
              std::move(v), std::move(ns)
            , std::move(bt), std::move(n)
            , std::move(vt), std::move(m)
            , std::move(ev), std::move(adef)
            , std::move(ad), std::move(av)
            , std::move(c));
        Network* result = net.release();
        return reinterpret_cast<dbcppp_Network*>(result);
    }
    DBCPPP_API void dbcppp_NetworkFree(dbcppp_Network* network)
    {
        std::unique_ptr<NetworkImpl> un(reinterpret_cast<NetworkImpl*>(network));
    }

    const char* dbcppp_NetworkGetVersion(struct dbcppp_Network* net)
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        return neti->getVersion().c_str();
    }
    bool dbcppp_NetworkHasNewSymbol(struct dbcppp_Network* net, const char* new_symbol)
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        return neti->hasNewSymbol(new_symbol);
    }
    void dbcppp_NetworkForEachNewSymbol(struct dbcppp_Network* net, void(*cb)(const char*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        neti->forEachNewSymbol(
            [&](const std::string& n)
            {
                cb(n.c_str());
            });
    }
    struct dbcppp_BitTiming* dbcppp_NetworkGetBitTiming(struct dbcppp_Network* net)
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const BitTiming& bt = neti->getBitTiming();
        return reinterpret_cast<dbcppp_BitTiming*>(&const_cast<BitTiming&>(bt));
    }
    struct dbcppp_Node* dbcppp_NetworkGetNodeByName(struct dbcppp_Network* net, const char* name)
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const Node* node = neti->getNodeByName(name);
        return reinterpret_cast<dbcppp_Node*>(const_cast<Node*>(node));
    }
    void dbcppp_network_for_each_node(struct dbcppp_Network* net, void(*cb)(dbcppp_Node*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        neti->forEachNode(
            [&](const Node& node)
            {
                cb(reinterpret_cast<dbcppp_Node*>(&const_cast<Node&>(node)));
            });
    }
    struct dbcppp_ValueTable* dbcppp_NetworkGetValueTableByName(struct dbcppp_Network* net, const char* name)
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const ValueTable* vt = neti->getValueTableByName(name);
        return reinterpret_cast<dbcppp_ValueTable*>(const_cast<ValueTable*>(vt));
    }
    struct dbcppp_ValueTable* dbcppp_NetworkFindValueTable(struct dbcppp_Network* net, bool(*pred)(struct dbcppp_ValueTable*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const ValueTable* vt = neti->findValueTable(
            [&](const ValueTable& vt)
            {
                return pred(reinterpret_cast<dbcppp_ValueTable*>(&const_cast<ValueTable&>(vt)));
            });
        return reinterpret_cast<dbcppp_ValueTable*>(const_cast<ValueTable*>(vt));
    }
    void dbcppp_NetworkForEachValueTable(struct dbcppp_Network* net, void(*cb)(struct dbcppp_ValueTable*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        neti->forEachValueTable(
            [&](const ValueTable& vt)
            {
                cb(reinterpret_cast<dbcppp_ValueTable*>(&const_cast<ValueTable&>(vt)));
            });
    }
    struct dbcppp_Message* dbcppp_NetworkGetMessageById(struct dbcppp_Network* net, uint64_t id)
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const Message* m = neti->getMessageById(id);
        return reinterpret_cast<dbcppp_Message*>(const_cast<Message*>(m));
    }
    struct dbcppp_Message* dbcppp_NetworkFindMessage(struct dbcppp_Network* net, bool(*pred)(struct dbcppp_Message*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const Message* m = neti->findMessage(
            [&](const Message& m)
            {
                return pred(reinterpret_cast<dbcppp_Message*>(&const_cast<Message&>(m)));
            });
        return reinterpret_cast<dbcppp_Message*>(const_cast<Message*>(m));
    }
    void dbcppp_NetworkForEachMessage(struct dbcppp_Network* net, void(*cb)(struct dbcppp_Message*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        neti->forEachMessage(
            [&](const Message& m)
            {
                cb(reinterpret_cast<dbcppp_Message*>(&const_cast<Message&>(m)));
            });
    }
    struct dbcppp_EnvironmentVariable* dbcppp_NetworkGetEnvironmentVariableByName(struct dbcppp_Network* net, const char* name)
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const EnvironmentVariable* ev = neti->getEnvironmentVariableByName(name);
        return reinterpret_cast<dbcppp_EnvironmentVariable*>(const_cast<EnvironmentVariable*>(ev));
    }
    struct dbcppp_EnvironmentVariable* dbcppp_NetworkFindEnvironmentVariable(struct dbcppp_Network* net, bool(*pred)(struct dbcppp_EnvironmentVariable*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const EnvironmentVariable* ev = neti->findEnvironmentVariable(
            [&](const EnvironmentVariable& ev)
            {
                return pred(reinterpret_cast<dbcppp_EnvironmentVariable*>(&const_cast<EnvironmentVariable&>(ev)));
            });
        return reinterpret_cast<dbcppp_EnvironmentVariable*>(const_cast<EnvironmentVariable*>(ev));
    }
    void dbcppp_NetworkForEachEnvironmentVariable(struct dbcppp_Network* net, void(*cb)(struct dbcppp_EnvironmentVariable*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        neti->forEachEnvironmentVariable(
            [&](const EnvironmentVariable& ev)
            {
                cb(reinterpret_cast<dbcppp_EnvironmentVariable*>(&const_cast<EnvironmentVariable&>(ev)));
            });
    }
    struct dbcppp_AttributeDefinition* dbcppp_NetworkGetAttributeDefinitionByName(struct dbcppp_Network* net, const char* name)
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const AttributeDefinition* ad = neti->getAttributeDefinitionByName(name);
        return reinterpret_cast<dbcppp_AttributeDefinition*>(const_cast<AttributeDefinition*>(ad));
    }
    struct dbcppp_AttributeDefinition* dbcppp_NetworkFindAttributeDefinition(struct dbcppp_Network* net, bool(*pred)(struct dbcppp_AttributeDefinition*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const AttributeDefinition* ad = neti->findAttributeDefinition(
            [&](const AttributeDefinition& ad)
            {
                return pred(reinterpret_cast<dbcppp_AttributeDefinition*>(&const_cast<AttributeDefinition&>(ad)));
            });
        return reinterpret_cast<dbcppp_AttributeDefinition*>(const_cast<AttributeDefinition*>(ad));
    }
    void dbcppp_NetworkForEachAttributeDefinition(struct dbcppp_Network* net, void(*cb)(struct dbcppp_AttributeDefinition*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        neti->forEachAttributeDefinition(
            [&](const AttributeDefinition& ad)
            {
                cb(reinterpret_cast<dbcppp_AttributeDefinition*>(&const_cast<AttributeDefinition&>(ad)));
            });
    }
    struct dbcppp_Attribute* dbcppp_NetworkGetAttributeDefaultByName(struct dbcppp_Network* net, const char* name)
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const Attribute* ad = neti->getAttributeDefaultByName(name);
        return reinterpret_cast<dbcppp_Attribute*>(const_cast<Attribute*>(ad));
    }
    struct dbcppp_Attribute* dbcppp_NetworkFindAttributeDefault(struct dbcppp_Network* net, bool(*pred)(struct dbcppp_Attribute*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const Attribute* ad = neti->findAttributeDefault(
            [&](const Attribute& ad)
            {
                return pred(reinterpret_cast<dbcppp_Attribute*>(&const_cast<Attribute&>(ad)));
            });
        return reinterpret_cast<dbcppp_Attribute*>(const_cast<Attribute*>(ad));
    }
    void dbcppp_NetworkForEachAttributeDefault(struct dbcppp_Network* net, void(*cb)(struct dbcppp_Attribute*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        neti->forEachAttributeDefault(
            [&](const Attribute& ad)
            {
                cb(reinterpret_cast<dbcppp_Attribute*>(&const_cast<Attribute&>(ad)));
            });
    }
    struct dbcppp_Attribute* dbcppp_NetworkGetAttributeValueByName(struct dbcppp_Network* net, const char* name)
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const Attribute* ad = neti->getAttributeValueByName(name);
        return reinterpret_cast<dbcppp_Attribute*>(const_cast<Attribute*>(ad));
    }
    struct dbcppp_Attribute* dbcppp_NetworkFindAttributeValue(struct dbcppp_Network* net, bool(*pred)(struct dbcppp_Attribute*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const Attribute* ad = neti->findAttributeValue(
            [&](const Attribute& ad)
            {
                return pred(reinterpret_cast<dbcppp_Attribute*>(&const_cast<Attribute&>(ad)));
            });
        return reinterpret_cast<dbcppp_Attribute*>(const_cast<Attribute*>(ad));
    }
    void dbcppp_NetworkForEachAttributeValue(struct dbcppp_Network* net, void(*cb)(struct dbcppp_Attribute*))
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        neti->forEachAttributeValue(
            [&](const Attribute& ad)
            {
                cb(reinterpret_cast<dbcppp_Attribute*>(&const_cast<Attribute&>(ad)));
            });
    }
    const char* dbcppp_NetworkGetComment(struct dbcppp_Network* net)
    {
        const NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        return neti->getComment().c_str();
    }

    uint64_t dbcppp_BitTimingGetBaudrate(struct dbcppp_BitTiming* bit_timing)
    {
        const BitTimingImpl* bti = reinterpret_cast<BitTimingImpl*>(bit_timing);
        return bti->getBaudrate();
    }
    uint64_t dbcppp_BitTimingGetBTR1(struct dbcppp_BitTiming* bit_timing)
    {
        const BitTimingImpl* bti = reinterpret_cast<BitTimingImpl*>(bit_timing);
        return bti->getBTR1();
    }
    uint64_t dbcppp_BitTimingGetBTR2(struct dbcppp_BitTiming* bit_timing)
    {
        const BitTimingImpl* bti = reinterpret_cast<BitTimingImpl*>(bit_timing);
        return bti->getBTR2();
    }
    
    const char* dbcppp_NodeGetName(struct dbcppp_Node* node)
    {
        const NodeImpl* ni = reinterpret_cast<NodeImpl*>(node);
        return ni->getName().c_str();
    }
    dbcppp_Attribute* dbcppp_NodeGetAttributeValueByName(struct dbcppp_Node* node, const char* attribute_name)
    {
        const NodeImpl* ni = reinterpret_cast<NodeImpl*>(node);
        return reinterpret_cast<dbcppp_Attribute*>(const_cast<Attribute*>(ni->getAttributeValueByName(attribute_name)));
    }
    dbcppp_Attribute* dbcppp_NodefindAttributeValue(struct dbcppp_Node* node, bool(*pred)(dbcppp_Attribute*))
    {
        const NodeImpl* ni = reinterpret_cast<NodeImpl*>(node);
        const Attribute* a = ni->findAttributeValue(
            [&](const Attribute& a)
            {
                return pred(reinterpret_cast<dbcppp_Attribute*>(const_cast<Attribute*>(&a)));
            });
        return reinterpret_cast<dbcppp_Attribute*>(const_cast<Attribute*>(a));
    }
    void dbcppp_NodeForEachAttributeValue(struct dbcppp_Node* node, void(*cb)(dbcppp_Attribute*))
    {
        const NodeImpl* ni = reinterpret_cast<NodeImpl*>(node);
        ni->forEachAttributeValue(
            [&](const Attribute& a)
            {
                cb(reinterpret_cast<dbcppp_Attribute*>(const_cast<Attribute*>(&a)));
            });
    }
    const char* dbcppp_NodeGetComment(struct dbcppp_Node* node)
    {
        const NodeImpl* ni = reinterpret_cast<NodeImpl*>(node);
        return ni->getComment().c_str();
    }

    const char* dbcppp_ValueTableGetName(dbcppp_ValueTable* value_table)
    {
        const ValueTableImpl* vti = reinterpret_cast<ValueTableImpl*>(value_table);
        return vti->getName().c_str();
    }
    dbcppp_SignalType* dbcppp_ValueTableGetSignalType(dbcppp_ValueTable* value_table)
    {
        const ValueTableImpl* vti = reinterpret_cast<ValueTableImpl*>(value_table);
        dbcppp_SignalType* result = nullptr;
        if (vti->getSignalType())
        {
            result = reinterpret_cast<dbcppp_SignalType*>(const_cast<SignalType*>(&*vti->getSignalType()));
        }
        return result;
    }
    void dbcppp_ValueTableForEachValueEncodingDescription(dbcppp_ValueTable* value_table, void(*cb)(double, const char*))
    {
        const ValueTableImpl* vti = reinterpret_cast<ValueTableImpl*>(value_table);
        vti->forEachValueEncodingDescription(
            [&](double value, const std::string& desc)
            {
                cb(value, desc.c_str());
            });
    }
}
