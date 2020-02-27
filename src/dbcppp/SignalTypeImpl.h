
#pragma once

#include "SignalType.h"
#include "ValueTable.h"

namespace dbcppp
{
	class SignalTypeImpl
		: public SignalType
	{
	public:
		virtual const std::string& getName() const override;
		virtual uint64_t getSignalSize() const override;
		virtual Signal::ByteOrder getByteOrder() const override;
		virtual Signal::ValueType getValueType() const override;
		virtual double getFactor() const override;
		virtual double getOffset() const override;
		virtual double getMinimum() const override;
		virtual double getMaximum() const override;
		virtual const std::string& getUnit() const override;
		virtual double getDefaultValue() const override;
		virtual const std::string& getValueTable() const override;

		std::string _name;
		uint64_t _signal_size;
		Signal::ByteOrder _byte_order;
		Signal::ValueType _value_type;
		double _factor;
		double _offset;
		double _minimum;
		double _maximum;
		std::string _unit;
		double _default_value;
		//ValueTable* _value_table;
		std::string _value_table;
	};
}