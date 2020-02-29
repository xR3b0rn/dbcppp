
#pragma once

#include <map>
#include <string>

#include "Helper.h"
#include "Export.h"
#include "Attribute.h"

namespace dbcppp
{
	class Network;
	class DBCPPP_API Node
	{
	public:
		virtual ~Node() = default;
		virtual const std::string& getName() const = 0;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const = 0;
		virtual const std::string& getComment() const = 0;
		
		void serializeToStream(std::ostream& os) const;
	};
}
