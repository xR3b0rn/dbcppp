
#pragma once

#include <boost/format.hpp>
#include <ostream>
#include "Export.h"

namespace dbcppp
{
    namespace Network2C
    {
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const Network& net);
    }
}
