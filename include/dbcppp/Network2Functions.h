
#pragma once

#include <boost/format.hpp>
#include <ostream>
#include <tuple>
#include <ostream>

#include "Export.h"
#include "../../include/dbcppp/Network.h"

namespace dbcppp
{
    namespace Network2C
    {
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const INetwork& net);
    }
    namespace Network2DBC
    {
        using na_t = std::tuple<const INetwork&, const IAttribute&>;
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const na_t& na);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const IAttributeDefinition& ad);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const IBitTiming& bt);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const IEnvironmentVariable& ev);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const IMessage& m);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const INetwork& net);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const INode& n);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const ISignal& s);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const ISignalType& st);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const IValueTable& vt);
    }
    namespace Network2Human
    {
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const INetwork& net);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const IMessage& msg);
    }
}
