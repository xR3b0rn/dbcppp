
#pragma once

#include <set>
#include <map>
#include <memory>
#include <string>
#include <cstddef>
#include <boost/optional.hpp>

#include "Export.h"
#include "Node.h"
#include "Attribute.h"


namespace dbcppp
{
	class DBCPPP_API Signal
	{
	public:
		enum class ErrorCode
			: uint64_t
		{
			NoError,
			SignalExceedsMessageSize,
			WrongBitSizeForExtendedDataType
		};
		enum class Multiplexer
		{
			NoMux, MuxSwitch, MuxValue
		};
		enum class ByteOrder
		{
			LittleEndian, BigEndian
		};
		enum class ValueType
		{
			Signed, Unsigned
		};
		enum class ExtendedValueType
		{
			Integer, Float, Double
		};
		
		static std::unique_ptr<Signal> create(
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
			, Signal::ExtendedValueType extended_value_type);

		virtual ~Signal() = default;
		virtual const std::string& getName() const = 0;
		virtual Multiplexer getMultiplexerIndicator() const = 0;
		virtual uint64_t getMultiplexerSwitchValue() const = 0;
		virtual uint64_t getStartBit() const = 0;
		virtual uint64_t getBitSize() const = 0;
		virtual ByteOrder getByteOrder() const = 0;
		virtual ValueType getValueType() const = 0;
		virtual double getFactor() const = 0;
		virtual double getOffset() const = 0;
		virtual double getMinimum() const = 0;
		virtual double getMaximum() const = 0;
		virtual std::string getUnit() const = 0;
		virtual bool hasReceiver(const std::string& name) const = 0;
		virtual std::vector<const std::string*> getReceivers() const = 0;
		virtual const std::string* getValueDescriptionById(double id) const = 0;
		virtual std::vector<std::pair<double, const std::string*>> getValueDescriptions() const = 0;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const = 0;
		virtual const std::string& getComment() const = 0;
		virtual ExtendedValueType getExtendedValueType() const = 0;
		virtual ErrorCode getError() const = 0;

		void serializeToStream(std::ostream& os) const;
		
		/// \brief Extracts the raw value from a given 8 byte array
		///
		/// This function uses a optimized method of reversing the byte order and extracting
		/// the value from the given data.
		/// !!! Note: This function takes at least 8 bytes, if you pass less, then you end in undefined behaviour! !!!
		///
		/// @param nbyte a n byte array (!!! at least 8 bytes !!!) which is representing the can data.
		///               the data must be in this order:
		///               bit_0  - bit_7:  bytes[0]
		///               bit_8  - bit_15: bytes[1]
		///               ...
		///               bit_n-7 - bit_n: bytes[n / 8]
		///               (like the Unix CAN frames does store the data)	
		inline double decode(const void* bytes) const noexcept { return _decode(this, bytes); }

		inline double raw_to_phys(double raw) const { return _raw_to_phys(this, raw); }
		inline double phys_to_raw(double phys) const { return _phys_to_raw(this, phys); }

	protected:
		// instead of using virtuals dynamic dispatching use function pointers
		double (*_decode)(const Signal* sig, const void* bytes) noexcept {nullptr};
		double (*_decode8)(const Signal* sig, const void* _8byte) noexcept {nullptr};
		double (*_decode64)(const Signal* sig, const void* _64byte) noexcept {nullptr};
		double (*_raw_to_phys)(const Signal* sig, double raw) noexcept {nullptr};
		double (*_phys_to_raw)(const Signal* sig, double phys) noexcept {nullptr};
	};
}
