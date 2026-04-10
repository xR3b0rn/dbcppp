#pragma once

#include <set>
#include <map>
#include <memory>
#include <string>
#include <cstddef>

#include "Export.h"
#include "Iterator.h"
#include "Node.h"
#include "Attribute.h"
#include "SignalMultiplexerValue.h"
#include "ValueEncodingDescription.h"

namespace dbcppp
{
    class DBCPPP_API ISignal
    {
    public:
        enum class EErrorCode
            : uint64_t
        {
            NoError,
            MaschinesFloatEncodingNotSupported = 1,
            MaschinesDoubleEncodingNotSupported = 2,
            SignalExceedsMessageSize = 4,
            WrongBitSizeForExtendedDataType = 8
        };
        enum class EMultiplexer
        {
            NoMux, MuxSwitch, MuxValue
        };
        enum class EByteOrder
        {
            BigEndian = 0,
            LittleEndian = 1
        };
        enum class EValueType
        {
            Signed, Unsigned
        };
        enum class EExtendedValueType
        {
            Integer, Float, Double
        };
        
        static std::unique_ptr<ISignal> Create(
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
            , std::vector<std::unique_ptr<IValueEncodingDescription>>&& value_encoding_description
            , std::string&& comment
            , EExtendedValueType extended_value_type
            , std::vector<std::unique_ptr<ISignalMultiplexerValue>>&& signal_multiplexer_values);
            
        virtual std::unique_ptr<ISignal> Clone() const = 0;

        virtual ~ISignal() = default;
        virtual const std::string& Name() const = 0;
        virtual EMultiplexer MultiplexerIndicator() const = 0;
        virtual uint64_t MultiplexerSwitchValue() const = 0;
        virtual uint64_t StartBit() const = 0;
        virtual uint64_t BitSize() const = 0;
        virtual EByteOrder ByteOrder() const = 0;
        virtual EValueType ValueType() const = 0;
        virtual double Factor() const = 0;
        virtual double Offset() const = 0;
        virtual double Minimum() const = 0;
        virtual double Maximum() const = 0;
        virtual const std::string& Unit() const = 0;
        virtual const std::string& Receivers_Get(std::size_t i) const = 0;
        virtual uint64_t Receivers_Size() const = 0;
        virtual const IValueEncodingDescription& ValueEncodingDescriptions_Get(std::size_t i) const = 0;
        virtual uint64_t ValueEncodingDescriptions_Size() const = 0;
        virtual const IAttribute& AttributeValues_Get(std::size_t i) const = 0;
        virtual uint64_t AttributeValues_Size() const = 0;
        virtual const std::string& Comment() const = 0;
        virtual EExtendedValueType ExtendedValueType() const = 0;
        virtual const ISignalMultiplexerValue& SignalMultiplexerValues_Get(std::size_t i) const = 0;
        virtual uint64_t SignalMultiplexerValues_Size() const = 0;

        virtual bool Error(EErrorCode code) const = 0;
        
        virtual bool operator==(const ISignal& rhs) const = 0;
        virtual bool operator!=(const ISignal& rhs) const = 0;
        
        /// \brief Extracts the raw value from a given n byte array
        ///
        /// This function uses a optimized method of reversing the byte order and extracting
        /// the value from the given data.
        /// !!! Note: This function takes at least 8 bytes and at least as many as the signal needs to be represented,
        ///     if you pass less, the program ends up in undefined behaviour! !!!
        ///
        /// @param nbyte a n byte array (!!! at least 8 bytes !!!) which is representing the can data.
        ///               the data must be in this order:
        ///               bit_0  - bit_7:  bytes[0]
        ///               bit_8  - bit_15: bytes[1]
        ///               ...
        ///               bit_n-7 - bit_n: bytes[n / 8]
        ///               (like the Unix CAN frame does store the data)
        using raw_t = uint64_t;
        inline raw_t Decode(const void* bytes) const noexcept { return _decode(this, bytes); }
        inline void Encode(raw_t raw, void* buffer) const noexcept { return _encode(this, raw, buffer); }

        inline double RawToPhys(raw_t raw) const noexcept { return _raw_to_phys(this, raw); }
        inline raw_t PhysToRaw(double phys) const noexcept { return _phys_to_raw(this, phys); }
        
        DBCPPP_MAKE_ITERABLE(ISignal, Receivers, std::string);
        DBCPPP_MAKE_ITERABLE(ISignal, ValueEncodingDescriptions, IValueEncodingDescription);
        DBCPPP_MAKE_ITERABLE(ISignal, AttributeValues, IAttribute);
        DBCPPP_MAKE_ITERABLE(ISignal, SignalMultiplexerValues, ISignalMultiplexerValue);

    protected:
        // instead of using virtuals dynamic dispatching use function pointers
        raw_t (*_decode)(const ISignal* sig, const void* bytes) noexcept {nullptr};
        void (*_encode)(const ISignal* sig, raw_t raw, void* buffer) noexcept {nullptr};
        double (*_raw_to_phys)(const ISignal* sig, raw_t raw) noexcept {nullptr};
        raw_t (*_phys_to_raw)(const ISignal* sig, double phys) noexcept {nullptr};
    };
}
