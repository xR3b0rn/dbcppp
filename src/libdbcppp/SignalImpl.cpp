#include <limits>
#include "Helper.h"
#include "SignalImpl.h"

using namespace dbcppp;

enum class Alignment
{
    size_inbetween_first_64_bit,
    signal_exceeds_64_bit_size_but_signal_fits_into_64_bit,
    signal_exceeds_64_bit_size_and_signal_does_not_fit_into_64_bit
};

template <Alignment aAlignment, ISignal::EByteOrder aByteOrder, ISignal::EValueType aValueType, ISignal::EExtendedValueType aExtendedValueType>
ISignal::raw_t template_decode(const ISignal* sig, const void* nbytes) noexcept
{
    const SignalImpl* sigi = static_cast<const SignalImpl*>(sig);
    uint64_t data;
    if constexpr (aAlignment == Alignment::signal_exceeds_64_bit_size_and_signal_does_not_fit_into_64_bit)
    {
        data = *reinterpret_cast<const uint64_t*>(&reinterpret_cast<const uint8_t*>(nbytes)[sigi->_byte_pos]);
        uint64_t data1 = reinterpret_cast<const uint8_t*>(nbytes)[sigi->_byte_pos + 8];
        if constexpr (aByteOrder == ISignal::EByteOrder::BigEndian)
        {
            //native_to_big_inplace(data);
            native_to_big_inplace(data);
            data &= sigi->_mask;
            data <<= sigi->_fixed_start_bit_0;
            data1 >>= sigi->_fixed_start_bit_1;
            data |= data1;
        }
        else
        {
            //native_to_little_inplace(data);
            native_to_little_inplace(data);
            data >>= sigi->_fixed_start_bit_0;
            data1 &= sigi->_mask;
            data1 <<= sigi->_fixed_start_bit_1;
            data |= data1;
        }
        if constexpr (aExtendedValueType == ISignal::EExtendedValueType::Float ||
            aExtendedValueType == ISignal::EExtendedValueType::Double)
        {
            return data;
        }
        if constexpr (aValueType == ISignal::EValueType::Signed)
        {
            if (data & sigi->_mask_signed)
            {
                data |= sigi->_mask_signed;
            }
        }
        return data;
    }
    else
    {
        if constexpr (aAlignment == Alignment::size_inbetween_first_64_bit)
        {
            data = *reinterpret_cast<const uint64_t*>(nbytes);
        }
        else
        {
            data = *reinterpret_cast<const uint64_t*>(&reinterpret_cast<const uint8_t*>(nbytes)[sigi->_byte_pos]);
        }
        if constexpr (aByteOrder == ISignal::EByteOrder::BigEndian)
        {
            //native_to_big_inplace(data);
            native_to_big_inplace(data);
        }
        else
        {
            //native_to_little_inplace(data);
            native_to_little_inplace(data);
        }
        if constexpr (aExtendedValueType == ISignal::EExtendedValueType::Double)
        {
            return data;
        }
        data >>= sigi->_fixed_start_bit_0;
    }
    data &= sigi->_mask;
    if constexpr (aExtendedValueType == ISignal::EExtendedValueType::Float)
    {
        return data;
    }
    if constexpr (aValueType == ISignal::EValueType::Signed)
    {
        // bit extending
        // trust the compiler to optimize this
        if (data & sigi->_mask_signed)
        {
            data |= sigi->_mask_signed;
        }
    }
    return data;
}

constexpr uint64_t enum_mask(Alignment a, ISignal::EByteOrder bo, ISignal::EValueType vt, ISignal::EExtendedValueType evt)
{
    uint64_t result = 0;
    switch (a)
    {
    case Alignment::size_inbetween_first_64_bit:                                    result |= 0b1; break;
    case Alignment::signal_exceeds_64_bit_size_but_signal_fits_into_64_bit:         result |= 0b10; break;
    case Alignment::signal_exceeds_64_bit_size_and_signal_does_not_fit_into_64_bit: result |= 0b100; break;
    }
    switch (bo)
    {
    case ISignal::EByteOrder::LittleEndian:                                           result |= 0b1000; break;
    case ISignal::EByteOrder::BigEndian:                                              result |= 0b10000; break;
    }
    switch (vt)
    {
    case ISignal::EValueType::Signed:                                                 result |= 0b100000; break;
    case ISignal::EValueType::Unsigned:                                               result |= 0b1000000; break;
    }
    switch (evt)
    {
    case ISignal::EExtendedValueType::Integer:                                        result |= 0b10000000; break;
    case ISignal::EExtendedValueType::Float:                                          result |= 0b100000000; break;
    case ISignal::EExtendedValueType::Double:                                         result |= 0b1000000000; break;
    }
    return result;
}
using decode_func_t = ISignal::raw_t (*)(const ISignal*, const void*) noexcept;
decode_func_t make_decode(Alignment a, ISignal::EByteOrder bo, ISignal::EValueType vt, ISignal::EExtendedValueType evt)
{
    constexpr auto si64b            = Alignment::size_inbetween_first_64_bit;
    constexpr auto se64bsbsfi64b    = Alignment::signal_exceeds_64_bit_size_but_signal_fits_into_64_bit;
    constexpr auto se64bsasdnfi64b  = Alignment::signal_exceeds_64_bit_size_and_signal_does_not_fit_into_64_bit;
    constexpr auto le               = ISignal::EByteOrder::LittleEndian;
    constexpr auto be               = ISignal::EByteOrder::BigEndian;
    constexpr auto sig              = ISignal::EValueType::Signed;
    constexpr auto usig             = ISignal::EValueType::Unsigned;
    constexpr auto i                = ISignal::EExtendedValueType::Integer;
    constexpr auto f                = ISignal::EExtendedValueType::Float;
    constexpr auto d                = ISignal::EExtendedValueType::Double;
    switch (enum_mask(a, bo, vt, evt))
    {
    case enum_mask(si64b, le, sig, i):            return template_decode<si64b, le, sig, i>;
    case enum_mask(si64b, le, sig, f):            return template_decode<si64b, le, sig, f>;
    case enum_mask(si64b, le, sig, d):            return template_decode<si64b, le, sig, d>;
    case enum_mask(si64b, le, usig, i):           return template_decode<si64b, le, usig, i>;
    case enum_mask(si64b, le, usig, f):           return template_decode<si64b, le, usig, f>;
    case enum_mask(si64b, le, usig, d):           return template_decode<si64b, le, usig, d>;
    case enum_mask(si64b, be, sig, i):            return template_decode<si64b, be, sig, i>;
    case enum_mask(si64b, be, sig, f):            return template_decode<si64b, be, sig, f>;
    case enum_mask(si64b, be, sig, d):            return template_decode<si64b, be, sig, d>;
    case enum_mask(si64b, be, usig, i):           return template_decode<si64b, be, usig, i>;
    case enum_mask(si64b, be, usig, f):           return template_decode<si64b, be, usig, f>;
    case enum_mask(si64b, be, usig, d):           return template_decode<si64b, be, usig, d>;
    case enum_mask(se64bsbsfi64b, le, sig, i):    return template_decode<se64bsbsfi64b, le, sig, i>;
    case enum_mask(se64bsbsfi64b, le, sig, f):    return template_decode<se64bsbsfi64b, le, sig, f>;
    case enum_mask(se64bsbsfi64b, le, sig, d):    return template_decode<se64bsbsfi64b, le, sig, d>;
    case enum_mask(se64bsbsfi64b, le, usig, i):   return template_decode<se64bsbsfi64b, le, usig, i>;
    case enum_mask(se64bsbsfi64b, le, usig, f):   return template_decode<se64bsbsfi64b, le, usig, f>;
    case enum_mask(se64bsbsfi64b, le, usig, d):   return template_decode<se64bsbsfi64b, le, usig, d>;
    case enum_mask(se64bsbsfi64b, be, sig, i):    return template_decode<se64bsbsfi64b, be, sig, i>;
    case enum_mask(se64bsbsfi64b, be, sig, f):    return template_decode<se64bsbsfi64b, be, sig, f>;
    case enum_mask(se64bsbsfi64b, be, sig, d):    return template_decode<se64bsbsfi64b, be, sig, d>;
    case enum_mask(se64bsbsfi64b, be, usig, i):   return template_decode<se64bsbsfi64b, be, usig, i>;
    case enum_mask(se64bsbsfi64b, be, usig, f):   return template_decode<se64bsbsfi64b, be, usig, f>;
    case enum_mask(se64bsbsfi64b, be, usig, d):   return template_decode<se64bsbsfi64b, be, usig, d>;
    case enum_mask(se64bsasdnfi64b, le, sig, i):  return template_decode<se64bsasdnfi64b, le, sig, i>;
    case enum_mask(se64bsasdnfi64b, le, sig, f):  return template_decode<se64bsasdnfi64b, le, sig, f>;
    case enum_mask(se64bsasdnfi64b, le, sig, d):  return template_decode<se64bsasdnfi64b, le, sig, d>;
    case enum_mask(se64bsasdnfi64b, le, usig, i): return template_decode<se64bsasdnfi64b, le, usig, i>;
    case enum_mask(se64bsasdnfi64b, le, usig, f): return template_decode<se64bsasdnfi64b, le, usig, f>;
    case enum_mask(se64bsasdnfi64b, le, usig, d): return template_decode<se64bsasdnfi64b, le, usig, d>;
    case enum_mask(se64bsasdnfi64b, be, sig, i):  return template_decode<se64bsasdnfi64b, be, sig, i>;
    case enum_mask(se64bsasdnfi64b, be, sig, f):  return template_decode<se64bsasdnfi64b, be, sig, f>;
    case enum_mask(se64bsasdnfi64b, be, sig, d):  return template_decode<se64bsasdnfi64b, be, sig, d>;
    case enum_mask(se64bsasdnfi64b, be, usig, i): return template_decode<se64bsasdnfi64b, be, usig, i>;
    case enum_mask(se64bsasdnfi64b, be, usig, f): return template_decode<se64bsasdnfi64b, be, usig, f>;
    case enum_mask(se64bsasdnfi64b, be, usig, d): return template_decode<se64bsasdnfi64b, be, usig, d>;
    }
    return nullptr;
}
decode_func_t make_decodeMuxSignal(Alignment a, ISignal::EByteOrder bo, ISignal::EValueType vt, ISignal::EExtendedValueType evt)
{
    constexpr auto si64b            = Alignment::size_inbetween_first_64_bit;
    constexpr auto se64bsbsfi64b    = Alignment::signal_exceeds_64_bit_size_but_signal_fits_into_64_bit;
    constexpr auto se64bsasdnfi64b  = Alignment::signal_exceeds_64_bit_size_and_signal_does_not_fit_into_64_bit;
    constexpr auto le               = ISignal::EByteOrder::LittleEndian;
    constexpr auto be               = ISignal::EByteOrder::BigEndian;
    constexpr auto sig              = ISignal::EValueType::Signed;
    constexpr auto usig             = ISignal::EValueType::Unsigned;
    constexpr auto i                = ISignal::EExtendedValueType::Integer;
    constexpr auto f                = ISignal::EExtendedValueType::Float;
    constexpr auto d                = ISignal::EExtendedValueType::Double;
    switch (enum_mask(a, bo, vt, evt))
    {
    case enum_mask(si64b, le, sig, i):            return template_decode<si64b, le, sig, i>;
    case enum_mask(si64b, le, sig, f):            return template_decode<si64b, le, sig, f>;
    case enum_mask(si64b, le, sig, d):            return template_decode<si64b, le, sig, d>;
    case enum_mask(si64b, le, usig, i):           return template_decode<si64b, le, usig, i>;
    case enum_mask(si64b, be, sig, i):            return template_decode<si64b, be, sig, i>;
    case enum_mask(si64b, be, usig, i):           return template_decode<si64b, be, usig, i>;
    case enum_mask(se64bsbsfi64b, le, sig, i):    return template_decode<se64bsbsfi64b, le, sig, i>;
    case enum_mask(se64bsbsfi64b, le, usig, i):   return template_decode<se64bsbsfi64b, le, usig, i>;
    case enum_mask(se64bsbsfi64b, be, sig, i):    return template_decode<se64bsbsfi64b, be, sig, i>;
    case enum_mask(se64bsbsfi64b, be, usig, i):   return template_decode<se64bsbsfi64b, be, usig, i>;
    case enum_mask(se64bsasdnfi64b, le, sig, i):  return template_decode<se64bsasdnfi64b, le, sig, i>;
    case enum_mask(se64bsasdnfi64b, le, usig, i): return template_decode<se64bsasdnfi64b, le, usig, i>;
    case enum_mask(se64bsasdnfi64b, be, sig, i):  return template_decode<se64bsasdnfi64b, be, sig, i>;
    case enum_mask(se64bsasdnfi64b, be, usig, i): return template_decode<se64bsasdnfi64b, be, usig, i>;
    }
    return nullptr;
}
void encode(const ISignal* sig, ISignal::raw_t raw, void* buffer) noexcept
{
    const SignalImpl* sigi = static_cast<const SignalImpl*>(sig);
    char* b = reinterpret_cast<char*>(buffer);
    if (sigi->ByteOrder() == ISignal::EByteOrder::BigEndian)
    {
        uint64_t src = sigi->StartBit();
        uint64_t dst = sigi->BitSize() - 1;
        for (uint64_t i = 0; i < sigi->BitSize(); i++)
        {
            if (raw & (1ull << dst))
            {
                b[src / 8] |= 1ull << (src % 8);
            }
            else
            {
                b[src / 8] &= ~(1ull << (src % 8));
            }
            if ((src % 8) == 0)
            {
                src += 15;
            }
            else
            {
                src--;
            }
            dst--;
        }
    }
    else
    {
        uint64_t src = sigi->StartBit();
        uint64_t dst = 0;
        for (uint64_t i = 0; i < sigi->BitSize(); i++)
        {
            if (raw & (1ull << dst))
            {
                b[src / 8] |= 1ull << (src % 8);
            }
            else
            {
                b[src / 8] &= ~(1ull << (src % 8));
            }
            src++;
            dst++;
        }
    }
}
template <class T>
double raw_to_phys(const ISignal* sig, ISignal::raw_t raw) noexcept
{
    const SignalImpl* sigi = static_cast<const SignalImpl*>(sig);
    double draw = double(*reinterpret_cast<T*>(&raw));
    return draw * sigi->Factor() + sigi->Offset();
}
template <class T>
ISignal::raw_t phys_to_raw(const ISignal* sig, double phys) noexcept
{
    const SignalImpl* sigi = static_cast<const SignalImpl*>(sig);
    T result = T((phys - sigi->Offset()) / sigi->Factor());
    return *reinterpret_cast<ISignal::raw_t*>(&result);
}
std::unique_ptr<ISignal> ISignal::Create(
      uint64_t message_size
    , std::string&& name
    , EMultiplexer multiplexer_indicator
    , uint64_t multiplexer_switch_value
    , uint64_t start_bit
    , uint64_t bit_size
    , EByteOrder byte_order
    , EValueType value_type
    , double factor
    , double offset
    , double minimum
    , double maximum
    , std::string&& unit
    , std::vector<std::string>&& receivers
    , std::vector<std::unique_ptr<IAttribute>>&& attribute_values
    , std::vector<std::unique_ptr<IValueEncodingDescription>>&& value_encoding_descriptions
    , std::string&& comment
    , EExtendedValueType extended_value_type
    , std::vector<std::unique_ptr<ISignalMultiplexerValue>>&& signal_multiplexer_values)
{
    std::unique_ptr<SignalImpl> result;
    std::vector<AttributeImpl> avs;
    for (auto& av : attribute_values)
    {
        avs.push_back(std::move(static_cast<AttributeImpl&>(*av)));
        av.reset(nullptr);
    }
    std::vector<ValueEncodingDescriptionImpl> veds;
    for (auto& ved : value_encoding_descriptions)
    {
        veds.push_back(std::move(static_cast<ValueEncodingDescriptionImpl&>(*ved)));
        ved.reset(nullptr);
    }
    std::vector<SignalMultiplexerValueImpl> smvs;
    for (auto& smv : signal_multiplexer_values)
    {
        smvs.push_back(std::move(static_cast<SignalMultiplexerValueImpl&>(*smv)));
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
        , std::move(veds)
        , std::move(comment)
        , extended_value_type
        , std::move(smvs));
    return result;
}


SignalImpl::SignalImpl(
      uint64_t message_size
    , std::string&& name
    , EMultiplexer multiplexer_indicator
    , uint64_t multiplexer_switch_value
    , uint64_t start_bit
    , uint64_t bit_size
    , EByteOrder byte_order
    , EValueType value_type
    , double factor
    , double offset
    , double minimum
    , double maximum
    , std::string&& unit
    , std::vector<std::string>&& receivers
    , std::vector<AttributeImpl>&& attribute_values
    , std::vector<ValueEncodingDescriptionImpl>&& value_encoding_descriptions
    , std::string&& comment
    , EExtendedValueType extended_value_type
    , std::vector<SignalMultiplexerValueImpl>&& signal_multiplexer_values)
    
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
    , _value_encoding_descriptions(std::move(value_encoding_descriptions))
    , _comment(std::move(comment))
    , _extended_value_type(std::move(extended_value_type))
    , _signal_multiplexer_values(std::move(signal_multiplexer_values))
    , _error(EErrorCode::NoError)
{
    message_size = message_size < 8 ? 8 : message_size;
    // check for out of frame size error
    switch (byte_order)
    {
    case EByteOrder::LittleEndian:
        if ((start_bit + bit_size) > message_size * 8)
        {
            SetError(EErrorCode::SignalExceedsMessageSize);
        }
        break;
    case EByteOrder::BigEndian:
        uint64_t fsize = bit_size + (7 - (start_bit % 8));
        int64_t fstart = int64_t(start_bit) - (start_bit % 8);
        if (fstart + ((fsize - 1) / 8) * 8 >= message_size * 8)
        {
            SetError(EErrorCode::SignalExceedsMessageSize);
        }
        break;
    }
    switch (extended_value_type)
    {
    case EExtendedValueType::Float:
        if (bit_size != 32)
        {
            SetError(EErrorCode::WrongBitSizeForExtendedDataType);
        }
        break;
    case EExtendedValueType::Double:
        if (bit_size != 64)
        {
            SetError(EErrorCode::WrongBitSizeForExtendedDataType);
        }
        break;
    }
    if (extended_value_type == EExtendedValueType::Float && !std::numeric_limits<float>::is_iec559)
    {
            SetError(EErrorCode::MaschinesFloatEncodingNotSupported);
    }
    if (extended_value_type == EExtendedValueType::Double && !std::numeric_limits<double>::is_iec559)
    {
            SetError(EErrorCode::MaschinesDoubleEncodingNotSupported);
    }

    // save some additional values to speed up decoding
    _mask =  (1ull << (_bit_size - 1ull) << 1ull) - 1;
    _mask_signed = ~((1ull << (_bit_size - 1ull)) - 1);

    _byte_pos = _start_bit / 8;

    uint64_t nbytes;
    if (_byte_order == EByteOrder::LittleEndian)
    {
        nbytes = (_start_bit % 8 + _bit_size + 7) / 8;
    }
    else
    {
        nbytes = (_bit_size + (7 - _start_bit % 8) + 7) / 8;
    }
    Alignment alignment = Alignment::size_inbetween_first_64_bit;
    // check whether the data is in the first 8 bytes
    // so we can optimize out one memory access
    if (_byte_pos + nbytes <= 8)
    {
        alignment = Alignment::size_inbetween_first_64_bit;
        if (_byte_order == EByteOrder::LittleEndian)
        {
            _fixed_start_bit_0 = _start_bit;
        }
        else
        {
            _fixed_start_bit_0 = (8 * (7 - (_start_bit / 8))) + (_start_bit % 8) - (_bit_size - 1);
        }
    }
    // check whether we can align the data on 64 bit
    else if (_byte_pos  % 8 + nbytes <= 8)
    {
        alignment = Alignment::signal_exceeds_64_bit_size_but_signal_fits_into_64_bit;
        // align the byte pos on 64 bit
        _byte_pos -= _byte_pos % 8;
        _fixed_start_bit_0 = _start_bit - _byte_pos * 8;
        if (_byte_order == EByteOrder::BigEndian)
        {
            _fixed_start_bit_0 = (8 * (7 - (_fixed_start_bit_0 / 8))) + (_fixed_start_bit_0 % 8) - (_bit_size - 1);
        }
    }
    // we aren't able to align the data on 64 bit, so check whether the data fits into on uint64_t
    else if (nbytes <= 8)
    {
        alignment = Alignment::signal_exceeds_64_bit_size_but_signal_fits_into_64_bit;
        _fixed_start_bit_0 = _start_bit - _byte_pos * 8;
        if (_byte_order == EByteOrder::BigEndian)
        {
            _fixed_start_bit_0 = (8 * (7 - (_fixed_start_bit_0 / 8))) + (_fixed_start_bit_0 % 8) - (_bit_size - 1);
        }
    }
    // we aren't able to align the data on 64 bit, and we aren't able to fit the data into one uint64_t
    // so we have to compose the resulting value
    else
    {
        alignment = Alignment::signal_exceeds_64_bit_size_and_signal_does_not_fit_into_64_bit;
        if (_byte_order == EByteOrder::BigEndian)
        {
            uint64_t nbits_last_byte = (7 - _start_bit % 8) + _bit_size - 64;
            _fixed_start_bit_0 = nbits_last_byte;
            _fixed_start_bit_1 = 8 - nbits_last_byte;
            _mask = (1ull << (_start_bit % 8 + 57)) - 1;
        }
        else
        {
            _fixed_start_bit_0 = _start_bit - _byte_pos * 8;
            _fixed_start_bit_1 = 64 - _start_bit % 8;
            uint64_t nbits_last_byte = _bit_size + _start_bit % 8 - 64;
            _mask = (1ull << nbits_last_byte) - 1ull;
        }
    }

    _decode = ::make_decode(alignment, _byte_order, _value_type, _extended_value_type);
    _encode = ::encode;
    switch (_extended_value_type)
    {
    case EExtendedValueType::Integer:
        switch (_value_type)
        {
        case EValueType::Signed:
            _raw_to_phys = ::raw_to_phys<int64_t>;
            _phys_to_raw = ::phys_to_raw<int64_t>;
            break;
        case EValueType::Unsigned:
            _raw_to_phys = ::raw_to_phys<uint64_t>;
            _phys_to_raw = ::phys_to_raw<uint64_t>;
            break;
        }
        break;
    case EExtendedValueType::Float:
        _raw_to_phys = ::raw_to_phys<float>;
        _phys_to_raw = ::phys_to_raw<float>;
        break;
    case EExtendedValueType::Double:
        _raw_to_phys = ::raw_to_phys<double>;
        _phys_to_raw = ::phys_to_raw<double>;
        break;
    }
}
std::unique_ptr<ISignal> SignalImpl::Clone() const
{
    return std::make_unique<SignalImpl>(*this);
}
const std::string& SignalImpl::Name() const
{
    return _name;
}
ISignal::EMultiplexer SignalImpl::MultiplexerIndicator() const
{
    return _multiplexer_indicator;
}
uint64_t SignalImpl::MultiplexerSwitchValue() const
{
    return _multiplexer_switch_value;
}
uint64_t SignalImpl::StartBit() const
{
    return _start_bit;
}
uint64_t SignalImpl::BitSize() const
{
    return _bit_size;
}
ISignal::EByteOrder SignalImpl::ByteOrder() const
{
    return _byte_order;
}
ISignal::EValueType SignalImpl::ValueType() const
{
    return _value_type;
}
double SignalImpl::Factor() const
{
    return _factor;
}
double SignalImpl::Offset() const
{
    return _offset;
}
double SignalImpl::Minimum() const
{
    return _minimum;
}
double SignalImpl::Maximum() const
{
    return _maximum;
}
const std::string& SignalImpl::Unit() const
{
    return _unit;
}
const std::string& SignalImpl::Receivers_Get(std::size_t i) const
{
    return _receivers[i];
}
uint64_t SignalImpl::Receivers_Size() const
{
    return _receivers.size();
}
const IValueEncodingDescription& SignalImpl::ValueEncodingDescriptions_Get(std::size_t i) const
{
    return _value_encoding_descriptions[i];
}
uint64_t SignalImpl::ValueEncodingDescriptions_Size() const
{
    return _value_encoding_descriptions.size();
}
const IAttribute& SignalImpl::AttributeValues_Get(std::size_t i) const
{
    return _attribute_values[i];
}
uint64_t SignalImpl::AttributeValues_Size() const
{
    return _attribute_values.size();
}
const std::string& SignalImpl::Comment() const
{
    return _comment;
}
ISignal::EExtendedValueType SignalImpl::ExtendedValueType() const
{
    return _extended_value_type;
}
const ISignalMultiplexerValue& SignalImpl::SignalMultiplexerValues_Get(std::size_t i) const
{
    return _signal_multiplexer_values[i];
}
uint64_t SignalImpl::SignalMultiplexerValues_Size() const
{
    return _signal_multiplexer_values.size();
}
bool SignalImpl::Error(EErrorCode code) const
{
    return code == _error || (uint64_t(_error) & uint64_t(code));
}
void SignalImpl::SetError(EErrorCode code)
{
    _error = EErrorCode(uint64_t(_error) | uint64_t(code));
}
bool SignalImpl::operator==(const ISignal& rhs) const
{
    bool equal = true;
    equal &= _name == rhs.Name();
    equal &= _multiplexer_indicator == rhs.MultiplexerIndicator();
    equal &= _multiplexer_switch_value == rhs.MultiplexerSwitchValue();
    equal &= _start_bit == rhs.StartBit();
    equal &= _bit_size == rhs.BitSize();
    equal &= _byte_order == rhs.ByteOrder();
    equal &= _value_type == rhs.ValueType();
    equal &= _factor == rhs.Factor();
    equal &= _offset == rhs.Offset();
    equal &= _minimum == rhs.Minimum();
    equal &= _maximum == rhs.Maximum();
    equal &= _unit == rhs.Unit();
    for (const auto& r : rhs.Receivers())
    {
        auto beg = _receivers.begin();
        auto end = _receivers.end();
        equal &= std::find(beg, end, r) != end;
    }
    for (const auto& attr : rhs.AttributeValues())
    {
        auto beg = _attribute_values.begin();
        auto end = _attribute_values.end();
        equal &= std::find(beg, end, attr) != end;
    }
    for (const auto& ved : rhs.ValueEncodingDescriptions())
    {
        auto beg = _value_encoding_descriptions.begin();
        auto end = _value_encoding_descriptions.end();
        equal &= std::find(beg, end, ved) != end;
    }
    equal &= _comment == rhs.Comment();
    equal &= _extended_value_type == rhs.ExtendedValueType();
    for (const auto& smv : rhs.SignalMultiplexerValues())
    {
        auto beg = _signal_multiplexer_values.begin();
        auto end = _signal_multiplexer_values.end();
        equal &= std::find(beg, end, smv) != end;
    }
    return equal;
}
bool SignalImpl::operator!=(const ISignal& rhs) const
{
    return !(*this == rhs);
}