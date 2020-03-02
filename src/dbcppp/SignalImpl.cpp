
#include "Helper.h"
#include "SignalImpl.h"

using namespace dbcppp;

template <dbcppp::Signal::ByteOrder aByteOrder, dbcppp::Signal::ValueType aValueType, dbcppp::Signal::ExtendedValueType aExtendedValueType>
double template_decode8(const Signal* sig, const void* _8byte) noexcept
{
	const SignalImpl* sigi = static_cast<const SignalImpl*>(sig);
	uint64_t data = *reinterpret_cast<const uint64_t*>(_8byte);
	if constexpr (aByteOrder == Signal::ByteOrder::BigEndian)
	{
		// only reverse byte order when native byte order isn't big endian
		boost::endian::native_to_big_inplace(data);
	}
	else
	{
		// only reverse byte order when native byte order isn't little endian
		boost::endian::native_to_little_inplace(data);
	}
	if constexpr (aExtendedValueType == Signal::ExtendedValueType::Double)
	{
		return *reinterpret_cast<double*>(&data);
	}
	data >>= sigi->_fixed_start_bit;
	if constexpr (aExtendedValueType == Signal::ExtendedValueType::Float)
	{
		return *reinterpret_cast<float*>(&data);
	}
	data &= sigi->_mask;
	if constexpr (aValueType == Signal::ValueType::Signed)
	{
		// bit extending
		data |= ~((data & sigi->_mask_signed) - 1);
		return double(*reinterpret_cast<int64_t*>(&data));
	}
	return double(data);
}
template <dbcppp::Signal::ByteOrder aByteOrder, dbcppp::Signal::ValueType aValueType, dbcppp::Signal::ExtendedValueType aExtendedValueType>
double template_decode64(const Signal* sig, const void* _64byte) noexcept
{
	const SignalImpl* sigi = static_cast<const SignalImpl*>(sig);
	uint8_t data8[64];
	std::memcpy(data8, _64byte, 64);
	if constexpr (aByteOrder == Signal::ByteOrder::BigEndian)
	{
		// only reverse byte order when native byte order isn't big endian
		native_to_big_inplace_64(reinterpret_cast<uint64_t*>(data8));
	}
	else
	{
		// only reverse byte order when native byte order isn't little endian
		native_to_little_inplace_64(reinterpret_cast<uint64_t*>(data8));
	}
	uint64_t data = *reinterpret_cast<uint64_t*>(&data8[sigi->_byte_pos_fd]);
	if constexpr (aExtendedValueType == Signal::ExtendedValueType::Double)
	{
		return *reinterpret_cast<double*>(&data);
	}
	data >>= sigi->_fixed_start_bit_fd;
	if constexpr (aExtendedValueType == Signal::ExtendedValueType::Float)
	{
		return *reinterpret_cast<float*>(&data);
	}
	data &= sigi->_mask;
	if constexpr (aValueType == Signal::ValueType::Signed)
	{
		// bit extending
		data |= ~((data & sigi->_mask_signed) - 1);
		return double(*reinterpret_cast<int64_t*>(&data));
	}
	return double(data);
}
double raw_to_phys(const Signal* sig, double raw) noexcept
{
	const SignalImpl* sigi = static_cast<const SignalImpl*>(sig);
	return raw * sigi->getFactor() + sigi->getOffset();
}
double phys_to_raw(const Signal* sig, double phys) noexcept
{
	const SignalImpl* sigi = static_cast<const SignalImpl*>(sig);
	return (phys - sigi->getOffset()) / sigi->getFactor();
}

std::unique_ptr<Signal> Signal::create(
	  uint64_t message_size
	, std::string&& name
	, Multiplexer multiplexer_indicator
	, uint64_t multiplexer_switch_value
	, uint64_t start_bit
	, uint64_t bit_size
	, ByteOrder byte_order
	, ValueType value_type
	, double factor
	, double offset
	, double minimum
	, double maximum
	, std::string&& unit
	, std::set<std::string>&& receivers
	, std::map<std::string, std::unique_ptr<Attribute>>&& attribute_values
	, std::map<double, std::string>&& value_descriptions
	, std::string&& comment
	, Signal::ExtendedValueType extended_value_type)
{
	std::unique_ptr<SignalImpl> result;
	std::map<std::string, AttributeImpl> avs;
	for (auto& av : attribute_values)
	{
		avs.insert(std::make_pair(av.first, std::move(*static_cast<AttributeImpl*>(av.second.get()))));
		av.second.reset(nullptr);
	}
	result = std::make_unique<SignalImpl>(
		  message_size
		, std::move(name)
		, multiplexer_indicator
		, multiplexer_switch_value
		, start_bit
		, bit_size
		, byte_order
		, value_type
		, factor
		, offset
		, minimum
		, maximum
		, std::move(unit)
		, std::move(receivers)
		, std::move(avs)
		, std::move(value_descriptions)
		, std::move(comment)
		, extended_value_type);
	if (result->getError() != SignalImpl::ErrorCode::NoError)
	{
		result = nullptr;
	}
	return result;
}
SignalImpl::SignalImpl(
	  uint64_t message_size
	, std::string&& name
	, Multiplexer multiplexer_indicator
	, uint64_t multiplexer_switch_value
	, uint64_t start_bit
	, uint64_t bit_size
	, ByteOrder byte_order
	, ValueType value_type
	, double factor
	, double offset
	, double minimum
	, double maximum
	, std::string&& unit
	, std::set<std::string>&& receivers
	, std::map<std::string, AttributeImpl>&& attribute_values
	, std::map<double, std::string>&& value_descriptions
	, std::string&& comment
	, ExtendedValueType extended_value_type)
	
	: _name(std::move(name))
	, _multiplexer_indicator(std::move(multiplexer_indicator))
	, _multiplexer_switch_value(std::move(multiplexer_switch_value))
	, _start_bit(std::move(start_bit))
	, _bit_size(std::move(bit_size))
	, _byte_order(std::move(byte_order))
	, _value_type(std::move(value_type))
	, _factor(std::move(factor))
	, _offset(std::move(offset))
	, _minimum(std::move(minimum))
	, _maximum(std::move(maximum))
	, _unit(std::move(unit))
	, _receivers(std::move(receivers))
	, _attribute_values(std::move(attribute_values))
	, _value_descriptions(std::move(value_descriptions))
	, _comment(std::move(comment))
	, _extended_value_type(std::move(extended_value_type))
	, _error(Signal::ErrorCode::NoError)
{
	// check for out of frame size error
	switch (byte_order)
	{
	case ByteOrder::LittleEndian:
		if ((start_bit + bit_size) > message_size * 8)
		{
			_error = ErrorCode::SignalExceedsMessageSize;
		}
		break;
	case ByteOrder::BigEndian:
		uint64_t fsize = bit_size + (7 - (start_bit % 8));
		int64_t fstart = int64_t(start_bit) - (start_bit % 8);
		if (fstart + ((fsize - 1) / 8) * 8 >= message_size * 8)
		{
			_error = ErrorCode::SignalExceedsMessageSize;
		}
		break;
	}
	switch (extended_value_type)
	{
	case Signal::ExtendedValueType::Float:
		if (bit_size != 32)
		{
			_error = ErrorCode::WrongBitSizeForExtendedDataType;
		}
		break;
	case Signal::ExtendedValueType::Double:
		if (bit_size != 64)
		{
			_error = ErrorCode::WrongBitSizeForExtendedDataType;
		}
		break;
	}

	// save some additional values to speed up decoding
	_mask =  (1ull << (_bit_size - 1ull) << 1ull) - 1;
	_mask_signed = 1ull << (_bit_size - 1ull);
	_fixed_start_bit =
		  _byte_order == dbcppp::Signal::ByteOrder::BigEndian
		? (8 * (7 - (_start_bit / 8))) + (_start_bit % 8) - (_bit_size - 1)
		: _start_bit;
	_fixed_start_bit_fd =
		  (_byte_order == dbcppp::Signal::ByteOrder::BigEndian
		? (8 * (63 - (_start_bit / 8))) + (_start_bit % 8) - (_bit_size - 1)
		: _start_bit);
	_byte_pos_fd = _fixed_start_bit_fd / 8;
	_fixed_start_bit_fd -= _byte_pos_fd * 8;

	// set decoding funciton
	if (_byte_order == Signal::ByteOrder::BigEndian &&
		_value_type == Signal::ValueType::Signed)
	{
		switch (_extended_value_type)
		{
		case Signal::ExtendedValueType::Integer:
		{
			_decode8 = template_decode8<Signal::ByteOrder::BigEndian, Signal::ValueType::Signed, Signal::ExtendedValueType::Integer>;
			_decode64 = template_decode64<Signal::ByteOrder::BigEndian, Signal::ValueType::Signed, Signal::ExtendedValueType::Integer>;
			break;
		}
		case Signal::ExtendedValueType::Float:
		{
			_decode8 = template_decode8<Signal::ByteOrder::BigEndian, Signal::ValueType::Signed, Signal::ExtendedValueType::Float>;
			_decode64 = template_decode64<Signal::ByteOrder::BigEndian, Signal::ValueType::Signed, Signal::ExtendedValueType::Float>;
			break;
		}
		case Signal::ExtendedValueType::Double:
		{
			_decode8 = template_decode8<Signal::ByteOrder::BigEndian, Signal::ValueType::Signed, Signal::ExtendedValueType::Double>;
			_decode64 = template_decode64<Signal::ByteOrder::BigEndian, Signal::ValueType::Signed, Signal::ExtendedValueType::Double>;
			break;
		}
		}
	}
	else if (_byte_order == dbcppp::Signal::ByteOrder::BigEndian &&
		_value_type == dbcppp::Signal::ValueType::Unsigned)
	{
		switch (_extended_value_type)
		{
		case Signal::ExtendedValueType::Integer:
		{
			_decode8 = template_decode8<Signal::ByteOrder::BigEndian, Signal::ValueType::Unsigned, Signal::ExtendedValueType::Integer>;
			_decode64 = template_decode64<Signal::ByteOrder::BigEndian, Signal::ValueType::Unsigned, Signal::ExtendedValueType::Integer>;
			break;
		}
		case Signal::ExtendedValueType::Float:
		{
			_decode8 = template_decode8<Signal::ByteOrder::BigEndian, Signal::ValueType::Unsigned, Signal::ExtendedValueType::Float>;
			_decode64 = template_decode64<Signal::ByteOrder::BigEndian, Signal::ValueType::Unsigned, Signal::ExtendedValueType::Float>;
			break;
		}
		case Signal::ExtendedValueType::Double:
		{
			_decode8 = template_decode8<Signal::ByteOrder::BigEndian, Signal::ValueType::Unsigned, Signal::ExtendedValueType::Double>;
			_decode64 = template_decode64<Signal::ByteOrder::BigEndian, Signal::ValueType::Unsigned, Signal::ExtendedValueType::Double>;
			break;
		}
		}
	}
	else if (_byte_order == Signal::ByteOrder::LittleEndian &&
		_value_type == Signal::ValueType::Signed)
	{
		switch (_extended_value_type)
		{
		case Signal::ExtendedValueType::Integer:
		{
			_decode8 = template_decode8<Signal::ByteOrder::LittleEndian, Signal::ValueType::Signed, Signal::ExtendedValueType::Integer>;
			_decode64 = template_decode64<Signal::ByteOrder::LittleEndian, Signal::ValueType::Signed, Signal::ExtendedValueType::Integer>;
			break;
		}
		case Signal::ExtendedValueType::Float:
		{
			_decode8 = template_decode8<Signal::ByteOrder::LittleEndian, Signal::ValueType::Signed, Signal::ExtendedValueType::Float>;
			_decode64 = template_decode64<Signal::ByteOrder::LittleEndian, Signal::ValueType::Signed, Signal::ExtendedValueType::Float>;
			break;
		}
		case Signal::ExtendedValueType::Double:
		{
			_decode8 = template_decode8<Signal::ByteOrder::LittleEndian, Signal::ValueType::Signed, Signal::ExtendedValueType::Double>;
			_decode64 = template_decode64<Signal::ByteOrder::LittleEndian, Signal::ValueType::Signed, Signal::ExtendedValueType::Double>;
			break;
		}
		}
	}
	else if (_byte_order == Signal::ByteOrder::LittleEndian &&
		_value_type == Signal::ValueType::Unsigned)
	{
		switch (_extended_value_type)
		{
		case Signal::ExtendedValueType::Integer:
		{
			_decode8 = template_decode8<Signal::ByteOrder::LittleEndian, Signal::ValueType::Unsigned, Signal::ExtendedValueType::Integer>;
			_decode64 = template_decode64<Signal::ByteOrder::LittleEndian, Signal::ValueType::Unsigned, Signal::ExtendedValueType::Integer>;
			break;
		}
		case Signal::ExtendedValueType::Float:
		{
			_decode8 = template_decode8<Signal::ByteOrder::LittleEndian, Signal::ValueType::Unsigned, Signal::ExtendedValueType::Float>;
			_decode64 = template_decode64<Signal::ByteOrder::LittleEndian, Signal::ValueType::Unsigned, Signal::ExtendedValueType::Float>;
			break;
		}
		case Signal::ExtendedValueType::Double:
		{
			_decode8 = template_decode8<Signal::ByteOrder::LittleEndian, Signal::ValueType::Unsigned, Signal::ExtendedValueType::Double>;
			_decode64 = template_decode64<Signal::ByteOrder::LittleEndian, Signal::ValueType::Unsigned, Signal::ExtendedValueType::Double>;
			break;
		}
		}
	}
	_raw_to_phys = ::raw_to_phys;
	_phys_to_raw = ::phys_to_raw;
}
const std::string& SignalImpl::getName() const
{
	return _name;
}
Signal::Multiplexer SignalImpl::getMultiplexerIndicator() const
{
	return _multiplexer_indicator;
}
uint64_t SignalImpl::getMultiplexerSwitchValue() const
{
	return _multiplexer_switch_value;
}
uint64_t SignalImpl::getStartBit() const
{
	return _start_bit;
}
uint64_t SignalImpl::getBitSize() const
{
	return _bit_size;
}
Signal::ByteOrder SignalImpl::getByteOrder() const
{
	return _byte_order;
}
Signal::ValueType SignalImpl::getValueType() const
{
	return _value_type;
}
double SignalImpl::getFactor() const
{
	return _factor;
}
double SignalImpl::getOffset() const
{
	return _offset;
}
double SignalImpl::getMinimum() const
{
	return _minimum;
}
double SignalImpl::getMaximum() const
{
	return _maximum;
}
std::string SignalImpl::getUnit() const
{
	return _unit;
}
bool SignalImpl::hasReceiver(const std::string& name) const
{
	return _receivers.find(name) != _receivers.end();
}
std::vector<const std::string*> SignalImpl::getReceivers() const
{
	std::vector<const std::string*> result;
	for (const auto& n : _receivers)
	{
		result.emplace_back(&n);
	}
	return result;
}
const std::string* SignalImpl::getValueDescriptionById(double id) const
{
	const std::string* result = nullptr;
	auto iter = _value_descriptions.find(id);
	if (iter != _value_descriptions.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<double, const std::string*>> SignalImpl::getValueDescriptions() const
{
	std::vector<std::pair<double, const std::string*>> result;
	for (auto& vd : _value_descriptions)
	{
		result.emplace_back(vd.first, &vd.second);
	}
	return result;
}
const Attribute* SignalImpl::getAttributeValueByName(const std::string& name) const
{
	const Attribute* result = nullptr;
	auto iter = _attribute_values.find(name);
	if (iter != _attribute_values.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const Attribute*>> SignalImpl::getAttributeValues() const
{
	std::vector<std::pair<std::string, const Attribute*>> result;
	for (auto& av : _attribute_values)
	{
		result.emplace_back(av.first, &av.second);
	}
	return result;
}
const std::string& SignalImpl::getComment() const
{
	return _comment;
}
Signal::ExtendedValueType SignalImpl::getExtendedValueType() const
{
	return _extended_value_type;
}
Signal::ErrorCode SignalImpl::getError() const
{
	return _error;
}

void Signal::serializeToStream(std::ostream& os) const
{
	os << "SG_ " << getName() << " ";
	switch (getMultiplexerIndicator())
	{
	case Signal::Multiplexer::MuxSwitch: os << "M "; break;
	case Signal::Multiplexer::MuxValue: os << "m" << getMultiplexerSwitchValue() << " "; break;
	}
	os << ": " << getStartBit() << "|" << getBitSize() << "@";
	switch (getByteOrder())
	{
	case Signal::ByteOrder::BigEndian: os << "0"; break;
	case Signal::ByteOrder::LittleEndian: os << "1"; break;
	}
	switch (getValueType())
	{
	case Signal::ValueType::Unsigned: os << "+ "; break;
	case Signal::ValueType::Signed: os << "- "; break;
	}
	os << "(" << getFactor() << "," << getOffset() << ") ";
	os << "[" << getMinimum() << "|" << getMaximum() << "] ";
	os << "\"" << getUnit() << "\"";
	for (auto& n : getReceivers())
	{
		os << " " << *n;
	}
}
