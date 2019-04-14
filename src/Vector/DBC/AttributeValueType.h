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
#include <vector>

namespace Vector {
namespace DBC {

/** Attribute Value Type */
class AttributeValueType {
public:
    /** Type */
    enum class Type {
        /** Integer */
        Int,

        /** Hex */
        Hex,

        /** Float */
        Float,

        /** String */
        String,

        /** Enum */
        Enum
    };

    /** @copydoc Type */
    Type type;

    /** Value Union */
    union {
        struct {
            int32_t minimum; /**< Min Value of type Type::Int */
            int32_t maximum; /**< Min Value of type Type::Int */
        } integerValue;

        struct {
            int32_t minimum; /**< Min Value of type Type::Hex */
            int32_t maximum; /**< Max Value of type Type::Hex */
        } hexValue;

        struct {
            double minimum; /**< Min Value of type Type::Float */
            double maximum; /**< Max Value of type Type::Float */
        } floatValue;

        // std::string has no default
    };

    /** Values of type AttributeValueType::Enum */
    std::vector<std::string> enumValues;
};

}
}
