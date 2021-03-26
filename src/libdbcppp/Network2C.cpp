
#include "NetworkImpl.h"
#include "../../include/dbcppp/Network2Functions.h"

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
    "}\n";
        
DBCPPP_API std::ostream& dbcppp::Network2C::operator<<(std::ostream& os, const Network& net)
{
    os << header;
    net.forEachMessage(
        [&](const Message& msg)
        {
            msg.forEachSignal(
                [&](const Signal& sig)
                {
                    const SignalImpl& sigi = static_cast<const SignalImpl&>(sig);
                    os << "uint64_t dbcppp_decode_" << msg.getName() << "_" << std::to_string(msg.getId()) <<
                        "_" << sig.getName() << "(const void* nbytes)\n"
                        "{\n" <<
                        "    uint64_t data;\n";
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
                            os << "    data = *reinterpret_cast<const uint64_t*>(nbytes);\n";
                        }
                        else
                        {
                            os << "data = *reinterpret_cast<const uint64_t*>(&reinterpret_cast<const uint8_t*>(nbytes)[" <<
                                sigi._byte_pos << "]);\n";
                        }
                        if (sig.getByteOrder() == Signal::ByteOrder::BigEndian)
                        {
                            os << "    data = dbcppp_native_to_big(data);\n";
                        }
                        else
                        {
                            os << "    data = dbcppp_native_to_little(data);\n";
                        }
                        if (sig.getExtendedValueType() == Signal::ExtendedValueType::Double)
                        {
                            os << "    return data;\n";
                        }
                        else
                        {
                            os << "    data >>= " << sigi._fixed_start_bit_0 << "ull;\n";
                            if (sig.getExtendedValueType() != Signal::ExtendedValueType::Float)
                            {
                                os << "    data &= " << sigi._mask << "ull;\n";
                                if (sig.getValueType() == Signal::ValueType::Signed)
                                {
                                    os << 
                                        "    if (data & " << sigi._mask_signed << "ull)\n"
                                        "    {\n"
                                        "        data |= " << sigi._mask_signed << "ull;\n"
                                        "    }\n"
                                        "    return data;\n";
                                }
                            }
                            os << "    return data;\n";
                        }
                    }
                    else
                    {
                        // Alignment::signal_exceeds_64_bit_size_and_signal_does_not_fit_into_64_bit;
                        os << "    hack.ui = *reinterpret_cast<const uint64_t*>(&reinterpret_cast<const uint8_t*>(nbytes)[" <<
                            sigi._byte_pos << "]);\n";
                        os << "    uint64_t data1 = reinterpret_cast<const uint8_t*>(nbytes)[" << sigi._byte_pos << " + 8];\n";
                        if (sig.getByteOrder() == Signal::ByteOrder::BigEndian)
                        {
                            os <<
                                "    data = dbcppp_native_to_big(data);\n"
                                "    data &= " << sigi._mask << "ull;\n"
                                "    data <<= " << sigi._fixed_start_bit_0 << "ull;\n"
                                "    data1 >>= " << sigi._fixed_start_bit_1 << "ull;\n"
                                "    data |= data1;\n";
                        }
                        else
                        {
                            os <<
                                "    data = dbcppp_native_to_little(data);\n"
                                "    data >>= " << sigi._fixed_start_bit_0 << "ull;\n"
                                "    data1 &= " << sigi._mask << "ull;\n"
                                "    data1 <<= " << sigi._fixed_start_bit_1 << "ull;\n"
                                "    data |= data1;)\n";
                        }
                        switch (sig.getExtendedValueType())
                        {
                        case Signal::ExtendedValueType::Integer:
                            if (sig.getValueType() == Signal::ValueType::Signed)
                            {
                                os <<
                                    "    if (data & " << sigi._mask_signed << "ull)\n"
                                    "    {\n"
                                    "        data |= " << sigi._mask_signed << "ull;\n"
                                    "    }\n"
                                    "    return data;\n";
                                os << "    return data;\n";
                            }
                            else
                            {
                                os << "    return data;\n";
                            }
                            break;
                        case Signal::ExtendedValueType::Float:
                            os << "    return data;\n";
                            break;
                        case Signal::ExtendedValueType::Double:
                            os << "    return data;\n";
                            break;
                        }
                    }
                    os << "}\n";
                    os <<
                        "double dbcppp_rawToPhys_" << msg.getName() << "_" << std::to_string(msg.getId()) <<
                        "_" + sig.getName() << "(uint64_t value)\n"
                        "{\n"
                        "    return value * " << sig.getFactor() << "+" << sig.getOffset() << ";\n"
                        "}\n";
                });

        });
    return os;
}
