
#pragma once

#include <vector>
#include <memory>

#include "../../include/dbcppp/Node.h"
#include "AttributeImpl.h"

namespace dbcppp
{
    class NodeImpl final
        : public Node
    {
    public:
        NodeImpl(
              std::string&& name
            , std::string&& comment
            , std::map<std::string, AttributeImpl>&& attribute_values);
            
        virtual std::unique_ptr<Node> clone() const override;

        virtual const std::string& getName() const override;
        virtual const std::string& getComment() const override;
        virtual const Attribute* getAttributeValueByName(const std::string& name) const override;
        virtual const Attribute* findAttributeValue(std::function<bool(const Attribute&)>&& pred) const override;
        virtual void forEachAttributeValue(std::function<void(const Attribute&)>&& cb) const override;

        std::string _name;
        std::string _comment;
        std::map<std::string, AttributeImpl> _attribute_values;
    };
    bool operator==(const dbcppp::NodeImpl& lhs, const std::string& rhs);
}
