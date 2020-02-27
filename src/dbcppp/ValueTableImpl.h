
#pragma once

#include <map>
#include "ValueTable.h"

namespace dbcppp
{
	class ValueTableImpl
		: public ValueTable
	{
	public:
		virtual const std::string& getName() const override;
		virtual std::vector<std::pair<uint64_t, const std::string*>> getValueDescriptions() const override;
		virtual const std::string* getValueDescriptionById(uint64_t id) const override;

		std::string _name;
		std::map<uint64_t, std::string> _value_descriptions;
	};
}
