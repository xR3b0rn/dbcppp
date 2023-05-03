#pragma once

#include <vector>
#include <memory>

#include "dbcppp/Node.h"
#include "AttributeImpl.h"

namespace dbcppp
{
    class NodeImpl final
        : public INode
    {
    public:
        NodeImpl(
              std::string&& name
            , std::string&& comment
            , std::vector<AttributeImpl>&& attribute_values);
            
        virtual std::unique_ptr<INode> Clone() const override;

        virtual const std::string& Name() const override;
        virtual const IAttribute& AttributeValues_Get(std::size_t i) const override;
        virtual uint64_t AttributeValues_Size() const override;
        virtual const std::string& Comment() const override;
        
        virtual bool operator==(const INode& rhs) const override;
        virtual bool operator!=(const INode& rhs) const override;

    private:
        std::string _name;
        std::string _comment;
        std::vector<AttributeImpl> _attribute_values;
    };
}
