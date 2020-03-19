
#include <boost/move/unique_ptr.hpp>
#include "MessageImpl.h"

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
{

    for(const auto& s : _signals)
    {
        auto& sig = s.second;

        std::set<std::string> receivers;
        sig.forEachReceiver
        (
                [&](const std::string& receiver)
                {
                    receivers.insert(const_cast<std::string&>(receiver));
                }
        );

        std::map<std::string, AttributeImpl> attribute_values;
        std::map<double, std::string> value_descriptions;

        auto _sig = Signal::create(
                message_size
                , sig.getName().data()
                , sig.getMultiplexerIndicator()
                , sig.getMultiplexerSwitchValue()
                , sig.getStartBit()
                , sig.getBitSize()
                , sig.getByteOrder()
                , sig.getValueType()
                , sig.getFactor()
                , sig.getOffset()
                , sig.getMinimum()
                , sig.getMaximum()
                , sig.getUnit()
                , std::move(receivers)
                , {}
                , {}
                , sig.getComment().data()
                , sig.getExtendedValueType()
        );
        _signals_order_by_start_bit.insert(std::make_pair(s.second.getStartBit(), std::move(*static_cast<SignalImpl*>(_sig.get()))));
    }
}
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
void MessageImpl::forEachMessageTransmitter(std::function<void(const std::string&)>&& cb) const
{
	for (const auto& n : _message_transmitters)
	{
		cb(n);
	}
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
const Signal* MessageImpl::findSignal(std::function<bool(const Signal&)>&& pred) const
{
	const Signal* result = nullptr;
	for (const auto& s : _signals)
	{
		if (pred(s.second))
		{
			result = &s.second;
			break;
		}
	}
	return result;
}
void MessageImpl::forEachSignal(std::function<void(const Signal&)>&& cb) const
{
	for (const auto& s : _signals)
	{
		cb(s.second);
	}
}
void MessageImpl::forEachSignalbyStartBit(std::function<void(const Signal&)>&& cb) const
{
    for (const auto& s : _signals_order_by_start_bit)
    {
        cb(s.second);
    }
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
const Attribute* MessageImpl::findAttributeValue(std::function<bool(const Attribute&)>&& pred) const
{
	const Attribute* result = nullptr;
	for (const auto& av : _attribute_values)
	{
		if (pred(av.second))
		{
			result = &av.second;
			break;
		}
	}
	return result;
}
void MessageImpl::forEachAttributeValue(std::function<void(const Attribute&)>&& cb) const
{
	for (const auto& av : _attribute_values)
	{
		cb(av.second);
	}
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
	forEachSignal(
		[&](const Signal& s)
		{
			os << "\n ";
			s.serializeToStream(os);
		});
}
