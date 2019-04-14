/*
 * Copyright (C) 2013-2019 Tobias Lorenz.
 * Contact: tobias.lorenz@gmx.net
 *
 * This file was orginally part of Tobias Lorenz's Toolkit (https://bitbucket.org/tobylorenz/vector_dbc/src/master/).
 * Now it's part of the dbcppp (DBC C++ parser) program with some minor changes in this file.
 *
 * GNU General Public License 3.0 Usage
 * This file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 */

#pragma once

#include <Vector/DBC/platform.h>

#include <cstdint>
#include <ostream>
#include <string>

#include <Vector/DBC/ByteOrder.h>
#include <Vector/DBC/ValueType.h>

namespace Vector {
namespace DBC {

/**
 * Signal Type (SGTYPE)
 */
class SignalType
{
public:
    /** Name */
    std::string name;

    /** Size */
    uint32_t size;

    /** Byte Order */
    ByteOrder byteOrder;

    /** Value Type */
    ValueType valueType;

    /** Factor */
    double factor;

    /** Offset */
    double offset;

    /** Minimum Physical Value */
    double minimum;

    /** Maximum Physical Value */
    double maximum;

    /** Unit */
    std::string unit;

    /** Default Value */
    double defaultValue;

    /** Value Table */
    std::string valueTable;
};
}
}
