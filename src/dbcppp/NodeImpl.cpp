
#include "NodeImpl.h"

using namespace dbcppp;

std::unique_ptr<Node> Node::create(
    std::string&& name,
    std::string&& comment,
    std::map<std::string, std::unique_ptr<Attribute>>&& attribute_values)
{
    std::map<std::string, AttributeImpl> avs;
    for (auto& av : attribute_values)
    {
        avs.insert(std::make_pair(av.first, std::move(*static_cast<AttributeImpl*>(av.second.get()))));
        av.second.reset(nullptr);
    }
    return std::make_unique<NodeImpl>(std::move(name), std::move(comment), std::move(avs));
}
NodeImpl::NodeImpl(std::string&& name, std::string&& comment, std::map<std::string, AttributeImpl>&& attribute_values)
    : _name(std::move(name))
    , _comment(std::move(comment))
    , _attribute_values(std::move(attribute_values))
{}
std::unique_ptr<Node> NodeImpl::clone() const
{
    return std::make_unique<NodeImpl>(*this);
}
const std::string& NodeImpl::getName() const
{
    return _name;
}
const std::string& NodeImpl::getComment() const
{
    return _comment;
}
const Attribute* NodeImpl::getAttributeValueByName(const std::string& name) const
{
    const Attribute* result = nullptr;
    auto iter = _attribute_values.find(name);
    if (iter != _attribute_values.end())
    {
        result = &iter->second;
    }
    return result;
}
const Attribute* NodeImpl::findAttributeValue(std::function<bool(const Attribute&)>&& pred) const
{
    const Attribute* result = nullptr;
    for (auto& av : _attribute_values)
    {
        if (pred(av.second))
        {
            result = &av.second;
            break;
        }
    }
    return result;
}
void NodeImpl::forEachAttributeValue(std::function<void(const Attribute&)>&& cb) const
{
    for (const auto& av : _attribute_values)
    {
        cb(av.second);
    }
}

void Node::serializeToStream(std::ostream& os) const
{
    os << getName();
}
