
#pragma once

#include <cstddef>
#include <string>
#include <memory>

#include "Export.h"
#include "Node.h"
#include "Signal.h"
#include "Attribute.h"

namespace dbcppp
{
	class DBCPPP_API Message
	{
	public:
		static std::unique_ptr<Message> create(
			  uint64_t id
			, std::string&& name
			, uint64_t message_size
			, std::string&& transmitter
			, std::set<std::string>&& message_transmitters
			, std::map<std::string, std::unique_ptr<Signal>>&& signals
			, std::map<std::string, std::unique_ptr<Attribute>>&& attribute_values
			, std::string&& comment);

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
		
		void serializeToStream(std::ostream& os) const;
	};
}
