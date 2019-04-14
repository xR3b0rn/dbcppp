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
#include <Vector/DBC/Signal.h>
#include <Vector/DBC/SignalGroup.h>

namespace Vector {
namespace DBC {

/**
 * Message (BO)
 */
class Message
{
public:
    /** Identifier (with bit 31 set this is extended CAN frame) */
    uint32_t id;

    /** Name */
    std::string name;

    /** Size */
    uint32_t size;

    /** Transmitter (empty string if the number of send nodes is zero or more than one) */
    std::string transmitter;

    /** Signals (SG) */
    std::map<std::string, Signal> signals;

    /** Message Transmitters (BO_TX_BU) */
    std::set<std::string> transmitters;

    /** Signal Groups (SIG_GROUP) */
    std::map<std::string, SignalGroup> signalGroups;

    /** Comment (CM) */
    std::string comment;

    /** Attribute Values (BA) */
    std::map<std::string, Attribute> attributeValues;
};
}
}
