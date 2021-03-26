
#pragma once

#include <ostream>
#include <tuple>
#include <ostream>

#include "Export.h"
#include "../../include/dbcppp/Network.h"

namespace dbcppp
{
    namespace Network2C
    {
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const Network& net);
    }
    namespace Network2DBC
    {
        using na_t = std::tuple<const Network&, const Attribute&>;
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const na_t& na);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const AttributeDefinition& ad);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const BitTiming& bt);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const EnvironmentVariable& ev);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const Message& m);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const Network& net);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const Node& n);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const Signal& s);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const SignalType& st);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const ValueTable& vt);
    }
    namespace Network2Human
    {
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const Network& net);
        DBCPPP_API std::ostream& operator<<(std::ostream& os, const Message& msg);
    }
}
