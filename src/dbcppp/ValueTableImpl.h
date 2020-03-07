
#pragma once

#include <map>
#include <memory>
#include "../../include/dbcppp/ValueTable.h"
#include "SignalTypeImpl.h"

namespace dbcppp
{
	class ValueTableImpl final
		: public ValueTable
	{
	public:
		ValueTableImpl(
			  std::string&& name
			, boost::optional<SignalTypeImpl>&& signal_type
			, std::map<double, std::string>&& value_encoding_descriptions);
		ValueTableImpl(ValueTableImpl&&) = default;

		virtual const std::string& getName() const override;
		virtual boost::optional<const SignalType&> getSignalType() const override;
		virtual std::vector<std::pair<double, const std::string*>> getValueEncodingDescriptions() const override;
		virtual const std::string* getValueEncodingDescriptions(double value) const override;

	private:
		std::string _name;
		boost::optional<SignalTypeImpl> _signal_type;
		std::map<double, std::string> _value_encoding_descriptions;
	};
}
