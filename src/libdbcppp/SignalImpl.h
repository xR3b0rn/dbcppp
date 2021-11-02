#pragma once

#include <string>
#include <memory>

#include <dbcppp/Signal.h>
#include <dbcppp/Node.h>
#include "AttributeImpl.h"
#include "SignalMultiplexerValueImpl.h"
#include "ValueEncodingDescriptionImpl.h"

namespace dbcppp
{
    class SignalImpl final
        : public ISignal
    {
    public:
        SignalImpl(
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
            , std::vector<SignalMultiplexerValueImpl>&& signal_multiplexer_values);
            
        virtual std::unique_ptr<ISignal> Clone() const override;

        virtual const std::string& Name() const override;
        virtual EMultiplexer MultiplexerIndicator() const override;
        virtual uint64_t MultiplexerSwitchValue() const override;
        virtual uint64_t StartBit() const override;
        virtual uint64_t BitSize() const override;
        virtual EByteOrder ByteOrder() const override;
        virtual EValueType ValueType() const override;
        virtual double Factor() const override;
        virtual double Offset() const override;
        virtual double Minimum() const override;
        virtual double Maximum() const override;
        virtual const std::string& Unit() const override;
        virtual const std::string& Receivers_Get(std::size_t i) const override;
        virtual uint64_t Receivers_Size() const override;
        virtual const IValueEncodingDescription& ValueEncodingDescriptions_Get(std::size_t i) const override;
        virtual uint64_t ValueEncodingDescriptions_Size() const override;
        virtual const IAttribute& AttributeValues_Get(std::size_t i) const override;
        virtual uint64_t AttributeValues_Size() const override;
        virtual const std::string& Comment() const override;
        virtual EExtendedValueType ExtendedValueType() const override;
        virtual const ISignalMultiplexerValue& SignalMultiplexerValues_Get(std::size_t i) const override;
        virtual uint64_t SignalMultiplexerValues_Size() const override;
        virtual bool Error(EErrorCode code) const override;
        
        virtual bool operator==(const ISignal& rhs) const override;
        virtual bool operator!=(const ISignal& rhs) const override;

    private:
        void SetError(EErrorCode code);

        std::string _name;
        EMultiplexer _multiplexer_indicator;
        uint64_t _multiplexer_switch_value;
        uint64_t _start_bit;
        uint64_t _bit_size;
        EByteOrder _byte_order;
        EValueType _value_type;
        double _factor;
        double _offset;
        double _minimum;
        double _maximum;
        std::string _unit;
        std::vector<std::string> _receivers;
        std::vector<AttributeImpl> _attribute_values;
        std::vector<ValueEncodingDescriptionImpl> _value_encoding_descriptions;
        std::string _comment;
        EExtendedValueType _extended_value_type;
        std::vector<SignalMultiplexerValueImpl> _signal_multiplexer_values;

    public:
        // for performance
        uint64_t _mask;
        uint64_t _mask_signed;
        uint64_t _fixed_start_bit_0;
        uint64_t _fixed_start_bit_1;
        uint64_t _byte_pos;

        EErrorCode _error;
    };
}