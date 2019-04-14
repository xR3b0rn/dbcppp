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
#include <string>

#include <Vector/DBC/AttributeObjectType.h>


namespace Vector {
namespace DBC {

/**
 * Attribute Value (BA)
 *
 * Attribute Default (BA_DEF_DEF) and
 * Attribute Default Value on Relation (BA_DEF_DEF_REL)
 */
class Attribute
{
public:
    /** Name */
    std::string name;

    /** Value Type */
    AttributeObjectType objectType;

    union {
        /** Integer Value of type AttributeValueType::Int */
        int32_t integerValue;

        /** Hex Value of type AttributeValueType::Hex */
        int32_t hexValue;

        /** Float Value of type AttributeValueType::Float */
        double floatValue;

        // std::string doesn't work in a union, so it's below

        /** Enum Value of type AttributeValueType::Enum (used only for BA enums) */
        int32_t enumValue;
    };

    /** String Value of type AttributeValueType::String (used only for BA_DEF_DEF enums) */
    std::string stringValue;
};

}
}
