
#pragma once

#include <vector>
#include <string>

#include "Export.h"
#include "SignalType.h"

namespace dbcppp
{
	class Network;
	class DBCPPP_API ValueTable
	{
	public:
		virtual ~ValueTable() = default;
		virtual const std::string& getName() const = 0;
		virtual boost::optional<const SignalType&> getSignalType() const = 0;
		virtual std::vector<std::pair<double, const std::string*>> getValueDescriptions() const = 0;
		virtual const std::string* getValueDescriptionById(double id) const = 0;
		
		void serializeToStream(std::ostream& os) const;
	};
}
