
#pragma once

#include "Message.h"
#include "SignalImpl.h"
#include "NodeImpl.h"
#include "AttributeImpl.h"

namespace dbcppp
{
	class MessageImpl
		: public Message
	{
	public:
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

		virtual Signal* addSignal(
			const std::string& name,
			Signal::ByteOrder byte_order,
			Signal::ValueType value_type,
			uint64_t bit_size, uint64_t start_bit,
			uint64_t message_size) override;
		virtual void removeSignal(const std::string& name) override;
		
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
