
#pragma once

#include <string>
#include <memory>

#include "Export.h"

#include "EndianConfig.h"

#ifdef _MSC_VER
#   include <stdlib.h>
#   define bswap_32(x) _byteswap_ulong(x)
#   define bswap_64(x) _byteswap_uint64(x)
#elif defined(__APPLE__)
    // Mac OS X / Darwin features
#   include <libkern/OSByteOrder.h>
#   define bswap_32(x) OSSwapInt32(x)
#   define bswap_64(x) OSSwapInt64(x)
#elif defined(__sun) || defined(sun)
#   include <sys/byteorder.h>
#   define bswap_32(x) BSWAP_32(x)
#   define bswap_64(x) BSWAP_64(x)
#elif defined(__FreeBSD__)
#   include <sys/endian.h>
#   define bswap_32(x) bswap32(x)
#   define bswap_64(x) bswap64(x)
#elif defined(__OpenBSD__)
#   include <sys/types.h>
#   define bswap_32(x) swap32(x)
#   define bswap_64(x) swap64(x)
#elif defined(__NetBSD__)
#   include <sys/types.h>
#   include <machine/bswap.h>
#   if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#       define bswap_32(x) bswap32(x)
#       define bswap_64(x) bswap64(x)
#   endif
#else
#   include <byteswap.h>
#endif

namespace dbcppp
{
    class NodeImpl;
    struct DBCPPP_API SharedNodeCmp
    {
        bool operator()(const NodeImpl& lhs, const NodeImpl& rhs) const;
    };
    inline void native_to_big_inplace(uint64_t& value)
    {
        if constexpr (dbcppp::Endian::Native == dbcppp::Endian::Little)
        {
            value = bswap_64(value);
        }
    }
    inline void native_to_little_inplace(uint64_t& value)
    {
        if constexpr (dbcppp::Endian::Native == dbcppp::Endian::Big)
        {
            value = bswap_64(value);
        }
    }
}
