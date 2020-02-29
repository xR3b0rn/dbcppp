
#pragma once

#include <cstddef>
#include <string>
#include <memory>

#include "Export.h"
#include "Helper.h"
#include "Node.h"
#include "Signal.h"
#include "Attribute.h"

namespace dbcppp
{
	class Network;
	class DBCPPP_API Message
	{
	public:
		virtual ~Message() = default;
		virtual uint64_t getId() const = 0;
		virtual const std::string& getName() const = 0;
		virtual uint64_t getMessageSize() const = 0;
		virtual const std::string& getTransmitter() const = 0;
		virtual bool hasMessageTransmitter(const std::string& name) const = 0;
		virtual std::vector<const std::string*> getMessageTransmitters() const = 0;
		virtual const Signal* getSignalByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const Signal*>> getSignals() const = 0;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const = 0;
		virtual const std::string& getComment() const = 0;
		
		virtual Signal* addSignal(
			const std::string& name,
			Signal::ByteOrder byte_order,
			Signal::ValueType value_type,
			uint64_t bit_size, uint64_t start_bit,
			uint64_t message_size) = 0;
		virtual void removeSignal(const std::string& name) = 0;
		
		void serializeToStream(std::ostream& os) const;
	};
}
