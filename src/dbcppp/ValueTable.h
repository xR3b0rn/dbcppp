
#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Export.h"
#include "SignalType.h"

namespace dbcppp
{
	class DBCPPP_API ValueTable
	{
	public:
		static std::unique_ptr<ValueTable> create(
			  std::string&& name
			, boost::optional<std::unique_ptr<SignalType>>&& signal_type
			, std::map<double, std::string>&& value_encoding_descriptions);

		virtual ~ValueTable() = default;
		virtual const std::string& getName() const = 0;
		virtual boost::optional<const SignalType&> getSignalType() const = 0;
		virtual std::vector<std::pair<double, const std::string*>> getValueEncodingDescriptions() const = 0;
		virtual const std::string* getValueEncodingDescriptions(double value) const = 0;
		
		void serializeToStream(std::ostream& os) const;
	};
}
