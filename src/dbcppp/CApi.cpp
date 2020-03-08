
#include "../../include/dbcppp/CApi.h"
#include "NetworkImpl.h"

using namespace dbcppp;

extern "C"
{
    struct dbcppp_Message* dbcppp_network_get_message_by_name(dbcppp_Network* net, uint64_t message_id)
    {
        NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        const Message* msgi = neti->getMessageById(message_id);
        return reinterpret_cast<dbcppp_Message*>(const_cast<Message*>(msgi));
    }
    void dbcppp_network_for_each_node(struct dbcppp_Network* net, void(*cb)(const char*, dbcppp_Node*))
    {
        NetworkImpl* neti = reinterpret_cast<NetworkImpl*>(net);
        neti->forEachNode(
            [&cb](const Node& node)
            {
                const NodeImpl& ni = static_cast<const NodeImpl&>(node);
                cb(ni.getName().c_str(), reinterpret_cast<dbcppp_Node*>(&const_cast<Node&>(node)));
            });
    }
}
