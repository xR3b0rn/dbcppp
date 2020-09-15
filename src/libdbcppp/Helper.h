
#pragma once

#include <string>
#include <memory>
#include <boost/endian/conversion.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/predef/hardware/simd.h>

#include "Export.h"

namespace dbcppp
{
    class NodeImpl;
    struct DBCPPP_API SharedNodeCmp
    {
        bool operator()(const NodeImpl& lhs, const NodeImpl& rhs) const;
    };
#if BOOST_ENDIAN_LITTLE_BYTE
    inline void native_to_big_inplace_64(uint64_t data[8]) noexcept
    {
        boost::endian::native_to_big_inplace(data[0]);
        boost::endian::native_to_big_inplace(data[1]);
        boost::endian::native_to_big_inplace(data[2]);
        boost::endian::native_to_big_inplace(data[3]);
        boost::endian::native_to_big_inplace(data[4]);
        boost::endian::native_to_big_inplace(data[5]);
        boost::endian::native_to_big_inplace(data[6]);
        boost::endian::native_to_big_inplace(data[7]);
        std::swap(data[0], data[7]);
        std::swap(data[1], data[6]);
        std::swap(data[2], data[5]);
        std::swap(data[3], data[4]);
    }
#else
    inline void native_to_big_inplace_64(uint64_t data[8]) noexcept
    {
    }
#endif
#if BOOST_ENDIAN_BIG_BYTE
    inline void native_to_little_inplace_64(uint64_t data[8]) noexcept
    {
        boost::endian::native_to_little_inplace(data[0]);
        boost::endian::native_to_little_inplace(data[1]);
        boost::endian::native_to_little_inplace(data[2]);
        boost::endian::native_to_little_inplace(data[3]);
        boost::endian::native_to_little_inplace(data[4]);
        boost::endian::native_to_little_inplace(data[5]);
        boost::endian::native_to_little_inplace(data[6]);
        boost::endian::native_to_little_inplace(data[7]);
        std::swap(data[0], data[7]);
        std::swap(data[1], data[6]);
        std::swap(data[2], data[5]);
        std::swap(data[3], data[4]);
    }
#else
    inline void native_to_little_inplace_64(uint64_t data[8]) noexcept
    {
    }
#endif
}
