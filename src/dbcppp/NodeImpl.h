
#pragma once

#include <vector>
#include <memory>

#include "Node.h"
#include "AttributeImpl.h"

namespace dbcppp
{
	class NodeImpl
		: public Node
	{
	public:
		NodeImpl(
			  std::string&& name
			, std::string&& comment
			, std::map<std::string, AttributeImpl>&& attribute_values);
		NodeImpl(NodeImpl&&) = default;
		NodeImpl& operator=(NodeImpl&&) = default;
		virtual const std::string& getName() const override;
		virtual const std::string& getComment() const override;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const override;

		std::string _name;
		std::string _comment;
		std::map<std::string, AttributeImpl> _attribute_values;
	};
	bool operator==(const dbcppp::NodeImpl& lhs, const std::string& rhs);
}
