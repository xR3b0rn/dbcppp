
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
		virtual void forEachMessageTransmitter(std::function<void(const std::string&)>&& cb) const override;
		virtual const Signal* getSignalByName(const std::string& name) const override;
		virtual const Signal* findSignal(std::function<bool(const Signal&)>&& pred) const override;
		virtual void forEachSignal(std::function<void(const Signal&)>&& cb) const override;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const override;
		virtual const Attribute* findAttributeValue(std::function<bool(const Attribute&)>&& pred) const override;
		virtual void forEachAttributeValue(std::function<void(const Attribute&)>&& cb) const override;
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
