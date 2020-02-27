
#pragma once

#include <set>
#include <map>
#include <memory>
#include <string>
#include <cstddef>


#include "Export.h"
#include "span.h"
#include "Helper.h"
#include "Node.h"
#include "Attribute.h"


namespace dbcppp
{
	class Message;
	class DBCPPP_API Signal
	{
	public:
		enum class ErrorCode
		{
			NoError,
			SignalExceedsMessageSize
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
		
		virtual ~Signal() = default;
		virtual const Message* getParentMessage() const = 0;
		virtual const std::string& getName() const = 0;
		virtual Multiplexer getMultiplexerIndicator() const = 0;
		virtual uint64_t getMultiplexerSwitchValue() const = 0;
		virtual uint64_t getStartBit() const = 0;
		virtual uint64_t getBitSize() const = 0;
		virtual ByteOrder getByteOrder() const = 0;
		virtual ValueType getValueType() = 0;
		virtual double getFactor() const = 0;
		virtual double getOffset() const = 0;
		virtual double getMinimum() const = 0;
		virtual double getMaximum() const = 0;
		virtual std::string getUnit() const = 0;
		virtual bool hasReceiver(const std::string& name) const = 0;
		virtual std::vector<const Node*> getReceivers() const = 0;
		virtual const std::string* getValueDescriptionById(uint64_t id) const = 0;
		virtual std::vector<std::pair<uint64_t, const std::string*>> getValueDescriptions() const = 0;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const = 0;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const = 0;
		virtual const std::string& getComment() const = 0;
		
		/// \brief Extracts the raw value from a given 8 byte array
		///
		/// This function uses a optimized method of reversing the byte order and extracting
		/// the value from the given data
		/// @param _8byte a 8 byte array (exactly 8 byte) which is representing the can data.
		///               the data must be in this order:
		///               bit0  - bit7:  _8byte[0]
		///               bit8  - bit15: _8byte[1]
		///               ...
		///               bit56 - bit63: _8byte[7]
		///               (like the Unix can_frame does store the data)
		inline double decode8(const void* _8byte) const noexcept { return _decode8(this, _8byte); }
		/// \brief Extracts the raw value from a given 64 byte array
		///
		/// This function uses a optimized method of reversing the byte order and extracting
		/// the value from the given data. In case of byte order missmatch, this function is much slower
		/// than decode8. A missmatch happens, e.g. when the Signal's byte order is BigEndian but the 
		/// systems byte order is LittleEndian. Then the byte reversing of decode64 will take some more
		/// instructions than the byte reversing of decode8.
		/// Note: Only data types with 64 bits are supported.
		/// @param _64byte a 64 byte array (exactly 64 byte) which is representing the can data.
		///               the data must be in this order:
		///               bit0   - bit7:   _64yte[0]
		///               bit8   - bit15:  _64byte[1]
		///               ...
		///               bit504 - bit511: _64byte[63]
		///               (like the Unix canfd_frame does store the data)
		inline double decode64(const void* _64byte) const noexcept { return _decode64(this, _64byte); }
		//void encode8(Signal::buffer_t* data, Signal::raw_t raw) const;
		//void encode64(Signal::buffer_t* data, Signal::raw_t raw) const;

		inline double raw_to_phys(double raw) const { return _raw_to_phys(this, raw); }
		inline double phys_to_raw(double phys) const { return _phys_to_raw(this, phys); }

	protected:
		// instead of using virtuals dynamic dispatching use function pointers
		double (*_decode8)(const Signal* sig, const void* _8byte) noexcept;
		double (*_decode64)(const Signal* sig, const void* _64byte) noexcept;
		double (*_raw_to_phys)(const Signal* sig, double raw) noexcept;
		double (*_phys_to_raw)(const Signal* sig, double phys) noexcept;
	};
}
