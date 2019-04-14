/*
 * Copyright (C) 2013-2019 Tobias Lorenz.
 * Contact: tobias.lorenz@gmx.net
 *
 * This file was orginally part of Tobias Lorenz's Toolkit (https://bitbucket.org/tobylorenz/vector_dbc/src/master/).
 * Now it's part of the dbcppp (DBC C++ parser) program with some minor changes.
 *
 * GNU General Public License 3.0 Usage
 * This file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 */

#include <algorithm>
#include <cmath>
#include <boost/predef/other/endian.h>

#include <Vector/DBC/Signal.h>
#include "Helper.h"

using namespace Vector::DBC;

double Signal::rawToPhysicalValue(double rawValue)
{
    /* physicalValue = rawValue * factor + offset */
    return rawValue * factor + offset;
}

double Signal::physicalToRawValue(double physicalValue)
{
    /* rawValue = (physicalValue - offset) / factor */
    return (physicalValue - offset) / factor;
}

double Signal::minimumRawValue()
{
    /* calculate minimum raw value */
    double minimumRawValue = 0.0;
    switch (extendedValueType)
	{
    case ExtendedValueType::Undefined:
    case ExtendedValueType::Integer:
        if (valueType == ValueType::Signed)
		{
            minimumRawValue = -(2 << (bitSize - 2)); // bitSize-- because shift instead of pow
        }
		else
		{
            minimumRawValue = 0.0;
        }
        break;
    case ExtendedValueType::Float:
        minimumRawValue = 3.4e-38;
        break;

    case ExtendedValueType::Double:
        minimumRawValue = 1.7e-308;
        break;
    }
    return minimumRawValue;
}

double Signal::maximumRawValue()
{
    /* calculate maximum raw value */
    double maximumRawValue = 0.0;
    switch (extendedValueType)
	{
    case ExtendedValueType::Undefined:
    case ExtendedValueType::Integer:
        if (valueType == ValueType::Signed)
		{
            maximumRawValue = (2ull << (bitSize - 2)) - 1; // bitSize-- because shift instead of pow
        } else {
            maximumRawValue = (2ull << (bitSize - 1)) - 1; // bitSize-- because shift instead of pow
        }
        break;
    case ExtendedValueType::Float:
        maximumRawValue = 3.4e38;
        break;
    case ExtendedValueType::Double:
        maximumRawValue = 1.7e308;
        break;
    }
    return maximumRawValue;
}
int64_t Signal::decode(const gsl::span<uint8_t> data)
{
	assert(data.size() > 0);
	int64_t d = *reinterpret_cast<const int64_t*>(data.data());
#ifdef BOOST_ENDIAN_LITTLE_BYTE
#elif BOOST_ENDIAN_BIG_BYTE
	switch (data.size())
	{
	case 1: d = data[sb] & 0xFF; break;
	case 2: d = reverse_bytes_2(d); break;
	case 3: d = reverse_bytes_3(d); break;
	case 4: d = reverse_bytes_4(d); break;
	case 5: d = reverse_bytes_5(d); break;
	case 6: d = reverse_bytes_6(d); break;
	case 7: d = reverse_bytes_7(d); break;
	case 8: d = reverse_bytes_8(d); break;
	}
	rawValue = reverse_bytes_8(rawValue);
#else
#	error "Vector::DBC::Signal Couldn't determine the systems endianess!"
#endif
	if (byteOrder == ByteOrder::LittleEndian)
	{
		d >>= startBit;
	}
	else
	{
		uint64_t sb = startBit / 8;
		uint64_t byte_size = (bitSize + 7 + ((startBit + 1 ) % 8)) / 8;
		d >>= sb  * 8;
		switch (byte_size)
		{
		case 1: d = data[sb] & 0xFF; break;
		case 2: d = reverse_bytes_2(d); break;
		case 3: d = reverse_bytes_3(d); break;
		case 4: d = reverse_bytes_4(d); break;
		case 5: d = reverse_bytes_5(d); break;
		case 6: d = reverse_bytes_6(d); break;
		case 7: d = reverse_bytes_7(d); break;
		case 8: d = reverse_bytes_8(d); break;
		}
		d >>= (startBit + 1) % 8;
	}
	int64_t mask = (2ull << (bitSize - 1)) - 1;
	d = d & mask;
	if (valueType == ValueType::Signed)
	{
		if ((d >> (bitSize - 1)) & 1)
		{
			d |= ~mask;
		}
	}
	return d; 
}
void Signal::encode(gsl::span<uint8_t> data, int64_t rawValue)
{
	assert(bitSize > 0);
    if (byteOrder == ByteOrder::BigEndian)
	{
        /* start with MSB */
        uint32_t srcBit = startBit;
        uint32_t dstBit = bitSize - 1;
        for (uint32_t i = 0; i < bitSize; ++i)
		{
            if (rawValue & (1ULL << dstBit))
			{
                data[srcBit / 8] |= (1 << (srcBit % 8));
            }
			else
			{
                data[srcBit / 8] &= ~(1 << (srcBit % 8));
            }
            if ((srcBit % 8) == 0)
			{
                srcBit += 15;
            }
			else
			{
                --srcBit;
            }
            --dstBit;
        }
    }
	else
	{
        uint32_t srcBit = startBit;
		uint32_t dstBit = 0;
        for (uint32_t i = 0; i < bitSize; ++i)
		{
            if (rawValue & (1ULL << dstBit))
			{
                data[srcBit / 8] |= (1 << (srcBit % 8));
			}
			else
			{
                data[srcBit / 8] &= ~(1 << (srcBit % 8));
            }
            ++srcBit;
            ++dstBit;
        }
    }
}
