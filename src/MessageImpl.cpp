#include <algorithm>
#include "MessageImpl.h"

using namespace dbcppp;


std::unique_ptr<IMessage> IMessage::Create(
      uint64_t id
    , std::string&& name
    , uint64_t message_size
    , std::string&& transmitter
    , std::vector<std::string>&& message_transmitters
    , std::vector<std::unique_ptr<ISignal>>&& signals_
    , std::vector<std::unique_ptr<IAttribute>>&& attribute_values
    , std::string&& comment
    , std::vector<std::unique_ptr<ISignalGroup>>&& signal_groups)
{
    std::vector<SignalImpl> ss;
    std::vector<AttributeImpl> avs;
    std::vector<SignalGroupImpl> sgs;
    for (auto& s : signals_)
    {
        ss.push_back(std::move(static_cast<SignalImpl&>(*s)));
        s.reset(nullptr);
    }
    for (auto& av : attribute_values)
    {
        avs.push_back(std::move(static_cast<AttributeImpl&>(*av)));
        av.reset(nullptr);
    }
    for (auto& sg : signal_groups)
    {
        sgs.push_back(std::move(static_cast<SignalGroupImpl&>(*sg)));
        sg.reset(nullptr);
    }
    return std::make_unique<MessageImpl>(
          id
        , std::move(name)
        , message_size
        , std::move(transmitter)
        , std::move(message_transmitters)
        , std::move(ss)
        , std::move(avs)
        , std::move(comment)
        , std::move(sgs));
}
MessageImpl::MessageImpl(
      uint64_t id
    , std::string&& name
    , uint64_t message_size
    , std::string&& transmitter
    , std::vector<std::string>&& message_transmitters
    , std::vector<SignalImpl>&& signals_
    , std::vector<AttributeImpl>&& attribute_values
    , std::string&& comment
    , std::vector<SignalGroupImpl>&& signal_groups)
    
    : _id(std::move(id))
    , _name(std::move(name))
    , _message_size(std::move(message_size))
    , _transmitter(std::move(transmitter))
    , _message_transmitters(std::move(message_transmitters))
    , _signals(std::move(signals_))
    , _attribute_values(std::move(attribute_values))
    , _comment(std::move(comment))
    , _signal_groups(std::move(signal_groups))
    , _mux_signal(nullptr)
    , _error(EErrorCode::NoError)
{
    bool have_mux_values = false;
    size_t num_mux_switches = 0;
    bool have_mux_values_without_advanced_mapping = false;
    for (const auto& sig : _signals)
    {
        if (std::underlying_type_t<ISignal::EMultiplexer>(sig.MultiplexerIndicator()) &
            std::underlying_type_t<ISignal::EMultiplexer>(ISignal::EMultiplexer::MuxValue))
        {
            have_mux_values = true;
            if (sig.SignalMultiplexerValues_Size() == 0)
            {
                have_mux_values_without_advanced_mapping = true;
            }
            else
            {
                for (const auto& multiplexor : sig.SignalMultiplexerValues())
                {
                    if (!std::any_of(_signals.begin(), _signals.end(), [&multiplexor](const SignalImpl& sig)
                        { return sig.Name() == multiplexor.SwitchName(); }))
                    {
                        _error = EErrorCode::MuxValeWithoutMuxSignal;
                    }
                }
            }
        }
        if (std::underlying_type_t<ISignal::EMultiplexer>(sig.MultiplexerIndicator()) &
            std::underlying_type_t<ISignal::EMultiplexer>(ISignal::EMultiplexer::MuxSwitch))
        {
            ++num_mux_switches;
        }
    }
    if (num_mux_switches > 1)
    {
        if (have_mux_values_without_advanced_mapping)
        {
            _error = EErrorCode::MuxValeWithoutMuxSignal;
        }
    }
    // MuxSignal property can be populated for simple multiplexing only.
    else if (num_mux_switches == 1)
    {
        for (const auto& sig : _signals)
        {
            if (uint64_t(sig.MultiplexerIndicator()) & uint64_t(ISignal::EMultiplexer::MuxSwitch))
            {
                _mux_signal = &sig;
            }
        }
        for (const auto& sig : _signals)
        {
            if (uint64_t(sig.MultiplexerIndicator()) & uint64_t(ISignal::EMultiplexer::MuxValue))
            {
                _mux_signal = &sig;
            }
        }
    }
    else if (num_mux_switches == 0)
    {
        if (have_mux_values)
        {
            _error = EErrorCode::MuxValeWithoutMuxSignal;
        }
    }
}
MessageImpl::MessageImpl(const MessageImpl& other)
{
    _id = other._id;
    _name = other._name;
    _message_size = other._message_size;
    _transmitter = other._transmitter;
    _message_transmitters = other._message_transmitters;
    _signals = other._signals;
    _attribute_values = other._attribute_values;
    _comment = other._comment;
    _mux_signal = nullptr;
    for (const auto& sig : _signals)
    {
        switch (sig.MultiplexerIndicator())
        {
        case ISignal::EMultiplexer::MuxSwitch:
            _mux_signal = &sig;
            break;
        }
    }
    _error = other._error;
}
MessageImpl& MessageImpl::operator=(const MessageImpl& other)
{
    _id = other._id;
    _name = other._name;
    _message_size = other._message_size;
    _transmitter = other._transmitter;
    _message_transmitters = other._message_transmitters;
    _signals = other._signals;
    _attribute_values = other._attribute_values;
    _comment = other._comment;
    _mux_signal = nullptr;
    for (const auto& sig : _signals)
    {
        switch (sig.MultiplexerIndicator())
        {
        case ISignal::EMultiplexer::MuxSwitch:
            _mux_signal = &sig;
            break;
        }
    }
    _error = other._error;
    return *this;
}
std::unique_ptr<IMessage> MessageImpl::Clone() const
{
    return std::make_unique<MessageImpl>(*this);
}
uint64_t MessageImpl::Id() const
{
    return _id;
}
const std::string& MessageImpl::Name() const
{
    return _name;
}
uint64_t MessageImpl::MessageSize() const
{
    return _message_size;
}
const std::string& MessageImpl::Transmitter() const
{
    return _transmitter;
}
const std::string& MessageImpl::MessageTransmitters_Get(std::size_t i) const
{
    return _message_transmitters[i];
}
uint64_t MessageImpl::MessageTransmitters_Size() const
{
    return _message_transmitters.size();
}
const ISignal& MessageImpl::Signals_Get(std::size_t i) const
{
    return _signals[i];
}
uint64_t MessageImpl::Signals_Size() const
{
    return _signals.size();
}
const IAttribute& MessageImpl::AttributeValues_Get(std::size_t i) const
{
    return _attribute_values[i];
}
uint64_t MessageImpl::AttributeValues_Size() const
{
    return _attribute_values.size();
}
const std::string& MessageImpl::Comment() const
{
    return _comment;
}
const ISignalGroup& MessageImpl::SignalGroups_Get(std::size_t i) const
{
    return _signal_groups[i];
}
uint64_t MessageImpl::SignalGroups_Size() const
{
    return _signal_groups.size();
}
const ISignal* MessageImpl::MuxSignal() const 
{
    return _mux_signal;
}
MessageImpl::EErrorCode MessageImpl::Error() const
{
    return _error;
}

const std::vector<SignalImpl>& MessageImpl::signals() const
{
    return _signals;
}
bool MessageImpl::operator==(const IMessage& rhs) const
{
    bool equal = true;
    equal &= _id == rhs.Id();
    equal &= _name == rhs.Name();
    equal &= _transmitter == rhs.Transmitter();
    for (const auto& msg_trans : rhs.MessageTransmitters())
    {
        auto beg = _message_transmitters.begin();
        auto end = _message_transmitters.end();
        equal &= std::find(beg, end, msg_trans) != _message_transmitters.end();
    }
    for (const auto& sig : rhs.Signals())
    {
        auto beg = _signals.begin();
        auto end = _signals.end();
        equal &= std::find(beg, end, sig) != _signals.end();
    }
    for (const auto& attr : rhs.AttributeValues())
    {
        auto beg = _attribute_values.begin();
        auto end = _attribute_values.end();
        equal &= std::find(beg, end, attr) != _attribute_values.end();
    }
    equal &= _comment == rhs.Comment();
    for (const auto& sg : rhs.SignalGroups())
    {
        auto beg = _signal_groups.begin();
        auto end = _signal_groups.end();
        equal &= std::find(beg, end, sg) != end;
    }
    return equal;
}
bool MessageImpl::operator!=(const IMessage& rhs) const
{
    return !(*this == rhs);
}