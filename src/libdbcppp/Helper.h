
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
    inline void native_to_big_inplace(uint64_t& data) noexcept
    {
        boost::endian::native_to_big_inplace(data);
    }
#else
    inline void native_to_big_inplace(uint64_t& data) noexcept
    {
    }
#endif
#if BOOST_ENDIAN_BIG_BYTE
    inline void native_to_little_inplace(uint64_t& data) noexcept
    {
        boost::endian::native_to_little_inplace(data);
    }
#else
    inline void native_to_little_inplace(uint64_t& data) noexcept
    {
    }
#endif
}
