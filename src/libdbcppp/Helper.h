
#pragma once

#include <string>
#include <memory>
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
#   defiee bswap_64(x) bswap64(x)
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

#include "Export.h"

namespace dbcppp
{
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

    class NodeImpl;
    struct DBCPPP_API SharedNodeCmp
    {
        bool operator()(const NodeImpl& lhs, const NodeImpl& rhs) const;
    };
    inline void native_to_big_inplace_64(uint64_t data[8]) noexcept
    {
        if constexpr (dbcppp::Endian::Native == dbcppp::Endian::Little)
        {
            native_to_big_inplace(data[0]);
            native_to_big_inplace(data[1]);
            native_to_big_inplace(data[2]);
            native_to_big_inplace(data[3]);
            native_to_big_inplace(data[4]);
            native_to_big_inplace(data[5]);
            native_to_big_inplace(data[6]);
            native_to_big_inplace(data[7]);
            std::swap(data[0], data[7]);
            std::swap(data[1], data[6]);
            std::swap(data[2], data[5]);
            std::swap(data[3], data[4]);
        }
    }
    inline void native_to_little_inplace_64(uint64_t data[8]) noexcept
    {
        if constexpr (dbcppp::Endian::Native == dbcppp::Endian::Big)
        {
            native_to_big_inplace(data[0]);
            native_to_big_inplace(data[1]);
            native_to_big_inplace(data[2]);
            native_to_big_inplace(data[3]);
            native_to_big_inplace(data[4]);
            native_to_big_inplace(data[5]);
            native_to_big_inplace(data[6]);
            native_to_big_inplace(data[7]);
            std::swap(data[0], data[7]);
            std::swap(data[1], data[6]);
            std::swap(data[2], data[5]);
            std::swap(data[3], data[4]);
        }
    }
}
