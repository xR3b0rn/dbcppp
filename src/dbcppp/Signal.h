
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
	struct Message;
	struct DBCPPP_EXPORT Signal
	{
		using buffer_t = uint64_t;
		using raw_t = int64_t;

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

		std::shared_ptr<Message> parent_message;
		std::string name;
		Multiplexer multiplexer_indicator;
		uint64_t multiplexer_switch_value;
		uint64_t start_bit;
		uint64_t bit_size;
		ByteOrder byte_order;
		ValueType value_type;
		double factor;
		double offset;
		double minimum;
		double maximum;
		std::string unit;
		std::set<std::shared_ptr<Node>, SharedNodeCmp> receivers;
		std::map<uint64_t, std::string> value_descriptions;
		std::map<std::string, Attribute> attribute_values;
		std::string comment;

		// instead of using virtuals dynamic dispatching use function pointers
		raw_t (*_decode8)(const Signal& sig, const void* _8byte) noexcept;
		raw_t (*_decode64)(const Signal& sig, const void* _64byte) noexcept;
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
		inline raw_t decode8(const void* _8byte) const noexcept;
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
		inline raw_t decode64(const void* _64byte) const noexcept;
		void encode(buffer_t* data, raw_t raw) const;
		double raw_to_phys(raw_t raw) const;
		raw_t phys_to_raw(double phys) const;
		
		// for performance
		void fix_performance_attributes();
		uint64_t mask;
		uint64_t mask_signed;
		uint64_t mask_signed_fd;
		uint64_t fixed_start_bit;
		uint64_t fixed_start_bit_fd;
		uint64_t byte_pos_fd;
	};
}
