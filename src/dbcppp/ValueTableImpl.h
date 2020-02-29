
#pragma once

#include <map>
#include <memory>
#include "ValueTable.h"
#include "SignalTypeImpl.h"

namespace dbcppp
{
	class ValueTableImpl
		: public ValueTable
	{
	public:
		virtual const std::string& getName() const override;
		virtual boost::optional<const SignalType&> getSignalType() const override;
		virtual std::vector<std::pair<double, const std::string*>> getValueDescriptions() const override;
		virtual const std::string* getValueDescriptionById(double id) const override;

		std::string _name;
		boost::optional<SignalTypeImpl> _signal_type;
		std::map<double, std::string> _value_descriptions;
	};
}
