#include "NodeImpl.h"

using namespace dbcppp;

std::unique_ptr<INode> INode::Create(
    std::string&& name,
    std::string&& comment,
    std::vector<std::unique_ptr<IAttribute>>&& attribute_values)
{
    std::vector<AttributeImpl> avs;
    for (auto& av : attribute_values)
    {
        avs.push_back(std::move(static_cast<AttributeImpl&>(*av)));
        av.reset(nullptr);
    }
    return std::make_unique<NodeImpl>(std::move(name), std::move(comment), std::move(avs));
}
NodeImpl::NodeImpl(std::string&& name, std::string&& comment, std::vector<AttributeImpl>&& attribute_values)
    : _name(std::move(name))
    , _comment(std::move(comment))
    , _attribute_values(std::move(attribute_values))
{}
std::unique_ptr<INode> NodeImpl::Clone() const
{
    return std::make_unique<NodeImpl>(*this);
}
const std::string& NodeImpl::Name() const
{
    return _name;
}
const std::string& NodeImpl::Comment() const
{
    return _comment;
}
const IAttribute& NodeImpl::AttributeValues_Get(std::size_t i) const
{
    return _attribute_values[i];
}
uint64_t NodeImpl::AttributeValues_Size() const
{
    return _attribute_values.size();
}
bool NodeImpl::operator==(const INode& rhs) const
{
    bool equal = true;
    equal &= _name == rhs.Name();
    equal &= _comment == rhs.Comment();
    for (const auto& attr : rhs.AttributeValues())
    {
        auto beg = _attribute_values.begin();
        auto end = _attribute_values.end();
        equal &= std::find(beg, end, attr) != end;
    }
    return equal;
}
bool NodeImpl::operator!=(const INode& rhs) const
{
    return !(*this == rhs);
}