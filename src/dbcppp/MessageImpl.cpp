
#include "MessageImpl.h"
#include <boost/move/unique_ptr.hpp>

using namespace dbcppp;


std::unique_ptr<Message> Message::create(
	  uint64_t id
	, std::string&& name
	, uint64_t message_size
	, std::string&& transmitter
	, std::set<std::string>&& message_transmitters
	, std::map<std::string, std::unique_ptr<Signal>>&& signals
	, std::map<std::string, std::unique_ptr<Attribute>>&& attribute_values
	, std::string&& comment)
{
	std::map<std::string, SignalImpl> ss;
	std::map<std::string, AttributeImpl> avs;
	for (auto& s : signals)
	{
		ss.insert(std::make_pair(s.first, std::move(static_cast<SignalImpl&>(*s.second))));
		s.second.reset(nullptr);
	}
	for (auto& av : attribute_values)
	{
		avs.insert(std::make_pair(av.first, std::move(static_cast<AttributeImpl&>(*av.second))));
		av.second.reset(nullptr);
	}
	return std::make_unique<MessageImpl>(
		  id
		, std::move(name)
		, message_size
		, std::move(transmitter)
		, std::move(message_transmitters)
		, std::move(ss)
		, std::move(avs)
		, std::move(comment));
}
MessageImpl::MessageImpl(
	  uint64_t id
	, std::string&& name
	, uint64_t message_size
	, std::string&& transmitter
	, std::set<std::string>&& message_transmitters
	, std::map<std::string, SignalImpl>&& signals
	, std::map<std::string, AttributeImpl>&& attribute_values
	, std::string&& comment)
	
	: _id(std::move(id))
	, _name(std::move(name))
	, _message_size(std::move(message_size))
	, _transmitter(std::move(transmitter))
	, _message_transmitters(std::move(message_transmitters))
	, _signals(std::move(signals))
	, _attribute_values(std::move(attribute_values))
	, _comment(std::move(comment))
{}
uint64_t MessageImpl::getId() const
{
	return _id;
}
const std::string& MessageImpl::getName() const
{
	return _name;
}
uint64_t MessageImpl::getMessageSize() const
{
	return _message_size;
}
const std::string& MessageImpl::getTransmitter() const
{
	return _transmitter;
}
bool MessageImpl::hasMessageTransmitter(const std::string& name) const
{
	return _message_transmitters.find(name) != _message_transmitters.end();
}
std::vector<const std::string*> MessageImpl::getMessageTransmitters() const
{
	std::vector<const std::string*> result;
	for (auto& n : _message_transmitters)
	{
		result.emplace_back(&n);
	}
	return result;
}
const Signal* MessageImpl::getSignalByName(const std::string& name) const
{
	const Signal* result = nullptr;
	auto iter = _signals.find(name);
	if (iter != _signals.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const Signal*>> MessageImpl::getSignals() const
{
	std::vector<std::pair<std::string, const Signal*>> result;
	for (const auto& s : _signals)
	{
		result.emplace_back(s.first, &s.second);
	}
	return result;
}
const Attribute* MessageImpl::getAttributeValueByName(const std::string& name) const
{
	const Attribute* result = nullptr;
	auto iter = _attribute_values.find(name);
	if (iter != _attribute_values.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const Attribute*>> MessageImpl::getAttributeValues() const
{
	std::vector<std::pair<std::string, const Attribute*>> result;
	for (auto& av : _attribute_values)
	{
		result.emplace_back(av.first, &av.second);
	}
	return result;
}
const std::string& MessageImpl::getComment() const
{
	return _comment;
}
const std::map<std::string, SignalImpl>& MessageImpl::signals() const
{
	return _signals;
}

void Message::serializeToStream(std::ostream& os) const
{
	os << "BO_ " << getId() << " " << getName() << ": " << getMessageSize() << " " << getTransmitter();
	for (const auto& s : getSignals())
	{
		os << "\n ";
		s.second->serializeToStream(os);
	}
}
