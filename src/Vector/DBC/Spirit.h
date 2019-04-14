/*
 * Copyright (C) 2019-2019 Julian Hindelang.
 *
 * This file is part of the dbcppp (DBC C++ parser) program.
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

#include <string>
#include <dbcppp_export.h>
#include "Network.h"

bool DBCPPP_EXPORT parse_dbc(const std::string& path, Vector::DBC::Network& m);
