
#pragma once

#include "NetworkImpl.h"
#include "../../include/dbcppp/Network2C.h"

using namespace dbcppp;
using namespace dbcppp::Network2C;

static const char* header =
    "#ifndef DBCPPP_BYTE_ORDER_LITTLE_ENDIAN\n"
    "#error \"Please pass -DDBCPPP_BYTE_ORDER_LITTLE_ENDIAN=<1|0>\"\n"
    "#endif\n"
    "#include <stdint.h>\n"
    "#define bswap_16(value) ((((value) & 0xff) << 8) | ((value) >> 8))\n"
    "#define bswap_32(value) \\\n"
    "    (((uint32_t)bswap_16((uint16_t)((value) & 0xffff)) << 16) | \\\n"
    "    (uint32_t)bswap_16((uint16_t)((value) >> 16)))\n"
    "#define bswap_64(value) \\\n"
    "    (((uint64_t)bswap_32((uint32_t)((value) & 0xffffffff)) \\\n"
    "    << 32) | \\\n"
    "    (uint64_t)bswap_32((uint32_t)((value) >> 32)))\n"
    "uint64_t dbcppp_native_to_big(uint64_t& v)\n"
    "{\n"
    "#if DBCPPP_BYTE_ORDER_LITTLE_ENDIAN == 1\n"
    "    return bswap_64(v);\n"
    "#else\n"
    "    return v;\n"
    "#endif\n"
    "}\n"
    "uint64_t dbcppp_native_to_little(uint64_t& v)\n"
    "{\n"
    "#if DBCPPP_BYTE_ORDER_LITTLE_ENDIAN == 1\n"
    "    return v;\n"
    "#else\n"
    "    return bswap_64(v);\n"
    "#endif\n"
    "}\n"
    "typedef union\n"
    "{\n"
    "    uint64_t ui;\n"
    "    int64_t i;\n"
    "    float f;\n"
    "    double d;\n"
    "} dbcppp_UnionHack;\n";
        
DBCPPP_API std::ostream& dbcppp::Network2C::operator<<(std::ostream& os, const Network& net)
{
    os << boost::format(header);
    net.forEachMessage(
        [&](const Message& msg)
        {
            msg.forEachSignal(
                [&](const Signal& sig)
                {
                    const SignalImpl& sigi = static_cast<const SignalImpl&>(sig);
                    os << boost::format(
                        "%1% dbcppp_decode_%2%(const void* nbytes)\n"
                        "{\n"
                        "    dbcppp_UnionHack hack;\n")
                        % "double"
                        % (msg.getName() + "_" + std::to_string(msg.getId()) + "_" + sig.getName());
                            
                    uint64_t nbytes;
                    if (sigi.getByteOrder() == Signal::ByteOrder::LittleEndian)
                    {
                        nbytes = (sigi.getStartBit() % 8 + sigi.getBitSize() + 7) / 8;
                    }
                    else
                    {
                        nbytes = (sigi.getBitSize() + (7 - sigi.getStartBit() % 8) + 7) / 8;
                    }
                    if (sigi._byte_pos + nbytes <= 8 || nbytes <= 8)
                    {
                        // Alignment::size_inbetween_first_64_bit; or
                        // Alignment::signal_exceeds_64_bit_size_but_signal_fits_into_64_bit;
                        if (sigi._byte_pos + nbytes <= 8)
                        {
                            os << boost::format("    hack.ui = *reinterpret_cast<const uint64_t*>(nbytes);\n");
                        }
                        else
                        {
                            os << boost::format(
                                "hack.ui = *reinterpret_cast<const uint64_t*>(&reinterpret_cast<const uint8_t*>(nbytes)[%1%]);\n")
                                % sigi._byte_pos;
                        }
                        if (sig.getByteOrder() == Signal::ByteOrder::BigEndian)
                        {
                            os << boost::format("    hack.ui = dbcppp_native_to_big(hack.ui);\n");
                        }
                        else
                        {
                            os << boost::format("    hack.ui = dbcppp_native_to_little(hack.ui);\n");
                        }
                        if (sig.getExtendedValueType() == Signal::ExtendedValueType::Double)
                        {
                            os << boost::format("    return hack.d;\n");
                        }
                        else
                        {
                            os << boost::format("    hack.ui >>= %1%ull;\n")
                                % sigi._fixed_start_bit_0;
                            if (sig.getExtendedValueType() == Signal::ExtendedValueType::Float)
                            {
                                os << boost::format("    return hack.d;\n");
                            }
                            else
                            {
                                os << boost::format("    hack.ui &= %1%ull;\n") % sigi._mask;
                                if (sig.getValueType() == Signal::ValueType::Signed)
                                {
                                    os << boost::format(
                                        "    if (hack.ui & %1%)\n"
                                        "    {\n"
                                        "        hack.ui |= %1%ull;\n"
                                        "    }\n"
                                        "    return double(hack.i);\n")
                                        % sigi._mask_signed;
                                    os << boost::format("    return double(hack.i);\n");
                                }
                                else
                                {
                                    os << boost::format("    return double(hack.ui);\n");
                                }
                            }
                        }
                    }
                    else
                    {
                        // Alignment::signal_exceeds_64_bit_size_and_signal_does_not_fit_into_64_bit;
                        os << boost::format(
                            "    hack.ui = *reinterpret_cast<const uint64_t*>(&reinterpret_cast<const uint8_t*>(nbytes)[%1%]);\n")
                            % sigi._byte_pos;
                        os << boost::format(
                            "    uint64_t data1 = reinterpret_cast<const uint8_t*>(nbytes)[%1% + 8];\n")
                            % sigi._byte_pos;
                        if (sig.getByteOrder() == Signal::ByteOrder::BigEndian)
                        {
                            os << boost::format(
                                "    hack.ui = dbcppp_native_to_big(hack.ui);\n"
                                "    hack.ui &= %1%ull;\n"
                                "    hack.ui <<= %2%ull;\n"
                                "    data1 >>= %3%ull;\n"
                                "    hack.ui |= data1;\n")
                                % sigi._mask
                                % sigi._fixed_start_bit_0
                                % sigi._fixed_start_bit_1;
                        }
                        else
                        {
                            os << boost::format(
                                "    hack.ui = dbcppp_native_to_little(hack.ui);\n"
                                "    hack.ui >>= %1%ull;\n"
                                "    data1 &= %2%ull;\n"
                                "    data1 <<= %3%ull;\n"
                                "    hack.ui |= data1;)\n")
                                % sigi._fixed_start_bit_0
                                % sigi._mask
                                % sigi._fixed_start_bit_1;
                        }
                        switch (sig.getExtendedValueType())
                        {
                        case Signal::ExtendedValueType::Integer:
                            if (sig.getValueType() == Signal::ValueType::Signed)
                            {
                                os << boost::format(
                                    "    if (hack.ui & %1%ull)\n"
                                    "    {\n"
                                    "        hack.ui |= %1%ull;\n"
                                    "    }\n"
                                    "    return double(hack.i);\n")
                                    % sigi._mask_signed;
                                os << boost::format("    return double(hack.i);\n");
                            }
                            else
                            {
                                os << boost::format("    return double(hack.ui);\n");
                            }
                            break;
                        case Signal::ExtendedValueType::Float:
                            os << boost::format("    return hack.f;\n");
                            break;
                        case Signal::ExtendedValueType::Double:
                            os << boost::format("    return hack.d;\n");
                            break;
                        }
                    }
                    os << boost::format("}\n");
                    os << boost::format(
                        "double dbcppp_rawToPhys_%1%(double value)\n"
                        "{\n"
                        "    return value * %2% + %3%;\n"
                        "}\n")
                        % (msg.getName() + "_" + std::to_string(msg.getId()) + "_" + sig.getName())
                        % sig.getFactor()
                        % sig.getOffset();
                });

        });
    return os;
}
