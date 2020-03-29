#ifndef DBCPPP_BYTE_ORDER_LITTLE_ENDIAN
#error "Please pass -DDBCPPP_BYTE_ORDER_LITTLE_ENDIAN=<1|0>"
#endif
#include <stdint.h>
#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)
#define bswap_16(value) ((((value) & 0xff) << 8) | ((value) >> 8))
#define bswap_32(value) \
    (((uint32_t)bswap_16((uint16_t)((value) & 0xffff)) << 16) | \
    (uint32_t)bswap_16((uint16_t)((value) >> 16)))
#define bswap_64(value) \
    (((uint64_t)bswap_32((uint32_t)((value) & 0xffffffff)) \
    << 32) | \
    (uint64_t)bswap_32((uint32_t)((value) >> 32)))
uint64_t dbcppp_native_to_big(uint64_t& v)
{
#if DBCPPP_BYTE_ORDER_LITTLE_ENDIAN == 1
    return bswap_64(v);
#else
    return v;
#endif
}
uint64_t dbcppp_native_to_little(uint64_t& v)
{
#if DBCPPP_BYTE_ORDER_LITTLE_ENDIAN == 1
    return v;
#else
    return bswap_64(v);
#endif
}
typedef union
{
    uint64_t ui;
    int64_t i;
    float f;
    double d;
} dbcppp_UnionHack;
double dbcppp_decode_test_msg_mux0_1_mux_switch_value(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 0ull;
    hack.ui &= 7ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_0(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 3ull;
    hack.ui &= 1023ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_1(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 13ull;
    hack.ui &= 1023ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_10(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 61ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_11(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 62ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_12(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 63ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_2(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 23ull;
    hack.ui &= 1023ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_3(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 33ull;
    hack.ui &= 1023ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_4(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 43ull;
    hack.ui &= 127ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_5(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 50ull;
    hack.ui &= 127ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_6(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 57ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_7(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 58ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_8(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 59ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s0_9(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 60ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s1_0(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 8ull;
    hack.ui &= 127ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s1_1(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 16ull;
    hack.ui &= 1023ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s1_2(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 26ull;
    hack.ui &= 1023ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s1_3(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 36ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s1_4(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 37ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s1_5(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 38ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s1_6(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 40ull;
    hack.ui &= 255ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s2_0(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 3ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s2_1(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 4ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s2_2(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 5ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s2_3(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 6ull;
    hack.ui &= 1023ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s2_4(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 16ull;
    hack.ui &= 1023ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s2_5(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 26ull;
    hack.ui &= 127ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s2_6(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 33ull;
    hack.ui &= 3ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s2_7(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 35ull;
    hack.ui &= 255ull;
    if (hack.ui & 18446744073709551488)
    {
        hack.ui |= 18446744073709551488ull;
    }
    return double(hack.i);
    return double(hack.i);
}
double dbcppp_decode_test_msg_mux0_1_s2_8(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 43ull;
    hack.ui &= 1023ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s2_9(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 53ull;
    hack.ui &= 63ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s3_0(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 8ull;
    hack.ui &= 255ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s3_1(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 16ull;
    hack.ui &= 2047ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s3_10(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 63ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s3_2(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 27ull;
    hack.ui &= 1023ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s3_3(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 37ull;
    hack.ui &= 1023ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s3_4(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 47ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s3_5(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 48ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s3_6(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 49ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s3_7(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 50ull;
    hack.ui &= 2047ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s3_8(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 61ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s3_9(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 62ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_0(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 3ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_1(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 8ull;
    hack.ui &= 127ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_10(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 32ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_11(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 33ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_12(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 34ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_13(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 35ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_14(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 36ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_15(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 37ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_16(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 38ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_17(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 40ull;
    hack.ui &= 63ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_18(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 46ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_2(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 16ull;
    hack.ui &= 127ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_3(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 24ull;
    hack.ui &= 3ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_4(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 26ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_5(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 27ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_6(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 28ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_7(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 29ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_8(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 30ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg_mux0_1_s4_9(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 31ull;
    hack.ui &= 1ull;
    return double(hack.ui);
}
double dbcppp_decode_test_msg0_0_s0(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 0ull;
    hack.ui &= 65535ull;
    if (hack.ui & 18446744073709518848)
    {
        hack.ui |= 18446744073709518848ull;
    }
    return double(hack.i);
    return double(hack.i);
}
double dbcppp_decode_test_msg0_0_s1(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    hack.ui >>= 0ull;
    return hack.d;
}
double dbcppp_decode_test_msg0_0_s2(const void* nbytes)
{
    dbcppp_UnionHack hack;
    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);
    hack.ui = dbcppp_native_to_little(hack.ui);
    return hack.d;
}