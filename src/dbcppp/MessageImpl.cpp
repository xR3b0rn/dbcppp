
#include "MessageImpl.h"

using namespace dbcppp;

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
bool MessageImpl::hasReceiver(const std::string& name) const
{
	return _receivers.find(name) != _receivers.end();
}
std::vector<const std::string*> MessageImpl::getReceivers() const
{
	std::vector<const std::string*> result;
	for (auto& n : _receivers)
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
	for (auto& s : _signals)
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
