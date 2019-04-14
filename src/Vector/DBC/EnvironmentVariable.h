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
#include <map>
#include <ostream>
#include <set>
#include <string>

#include <Vector/DBC/Attribute.h>
#include <Vector/DBC/ValueDescriptions.h>

namespace Vector {
namespace DBC {

/**
 * Environment Variable (EV)
 */
class EnvironmentVariable
{
public:
    /** Name */
    std::string name;

    /** Type */
    enum class Type : char {
        /** Integer */
        Integer = '0', // on write for Integer/String/Data

        /** Float */
        Float = '1', // on write for Float

        /** String */
        String = 's', // set on read when AccessType & 0x8000

        /** Data */
        Data = 'd' // set on read when ENVVAR_DATA exists
    };

    /** Type */
    Type type;

    /** Minimum Physical Value */
    double minimum;

    /** Maximum Physical Value */
    double maximum;

    /** Unit */
    std::string unit;

    /** Initial Value */
    double initialValue;

    /** Identifier */
    uint32_t id;

    /** Access Type */
    enum class AccessType : uint16_t {
        /** Unrestricted */
        Unrestricted = 0x0000,

        /** Read */
        Read = 0x0001,

        /** Write */
        Write = 0x0002,

        /** Read and Write */
        ReadWrite = 0x0003
    };

    /** Access Type */
    AccessType accessType;

    /** Access Nodes */
    std::set<std::string> accessNodes;

    /** Value Descriptions (VAL) */
    ValueDescriptions valueDescriptions;

    /** Environment Variables Data (ENVVAR_DATA) */
    uint32_t dataSize;

    /** Comment (CM) */
    std::string comment;

    /** Attribute Values (BA) */
    std::map<std::string, Attribute> attributeValues;
};
}
}
