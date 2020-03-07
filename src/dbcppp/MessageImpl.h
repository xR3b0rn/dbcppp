
#pragma once

#include "../../include/dbcppp/Message.h"
#include "SignalImpl.h"
#include "NodeImpl.h"
#include "AttributeImpl.h"

namespace dbcppp
{
	class MessageImpl final
		: public Message
	{
	public:
		MessageImpl(
			  uint64_t id
			, std::string&& name
			, uint64_t message_size
			, std::string&& transmitter
			, std::set<std::string>&& message_transmitters
			, std::map<std::string, SignalImpl>&& signals
			, std::map<std::string, AttributeImpl>&& attribute_values
			, std::string&& comment);
		MessageImpl(MessageImpl&&) = default;
		MessageImpl& operator=(MessageImpl&&) = default;
		virtual uint64_t getId() const override;
		virtual const std::string& getName() const override;
		virtual uint64_t getMessageSize() const override;
		virtual const std::string& getTransmitter() const override;
		virtual bool hasMessageTransmitter(const std::string& name) const override;
		virtual std::vector<const std::string*> getMessageTransmitters() const override;
		virtual const Signal* getSignalByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const Signal*>> getSignals() const override;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const override;
		virtual const std::string& getComment() const override;
		
		const std::map<std::string, SignalImpl>& signals() const;
		
	private:
		uint64_t _id;
		std::string _name;
		uint64_t _message_size;
		std::string _transmitter;
		std::set<std::string> _message_transmitters;
		std::map<std::string, SignalImpl> _signals;
		std::map<std::string, AttributeImpl> _attribute_values;
		std::string _comment;
	};
}
