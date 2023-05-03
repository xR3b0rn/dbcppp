
#include "NetworkImpl.h"
#include "dbcppp/Network2Functions.h"

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
        
DBCPPP_API std::ostream& dbcppp::Network2C::operator<<(std::ostream& os, const INetwork& net)
{
    os << boost::format(header);
    for (const auto& msg : net.Messages())
    {
        for (const auto& sig : msg.Signals())
        {
            const SignalImpl& sigi = static_cast<const SignalImpl&>(sig);
            os << boost::format(
                "uint64_t dbcppp_decode_%s(const void* nbytes)\n"
                "{\n"
                "    uint64_t data;\n")
                % (msg.Name() + "_" + std::to_string(msg.Id()) + "_" + sig.Name());
                            
            uint64_t nbytes;
            if (sigi.ByteOrder() == ISignal::EByteOrder::LittleEndian)
            {
                nbytes = (sigi.StartBit() % 8 + sigi.BitSize() + 7) / 8;
            }
            else
            {
                nbytes = (sigi.BitSize() + (7 - sigi.StartBit() % 8) + 7) / 8;
            }
            if (sigi._byte_pos + nbytes <= 8 || nbytes <= 8)
            {
                // Alignment::size_inbetween_first_64_bit; or
                // Alignment::signal_exceeds_64_bit_size_but_signal_fits_into_64_bit;
                if (sigi._byte_pos + nbytes <= 8)
                {
                    os << boost::format("    data = *reinterpret_cast<const uint64_t*>(nbytes);\n");
                }
                else
                {
                    os << boost::format(
                        "data = *reinterpret_cast<const uint64_t*>(&reinterpret_cast<const uint8_t*>(nbytes)[%1%]);\n")
                        % sigi._byte_pos;
                }
                if (sig.ByteOrder() == ISignal::EByteOrder::BigEndian)
                {
                    os << boost::format("    data = dbcppp_native_to_big(data);\n");
                }
                else
                {
                    os << boost::format("    data = dbcppp_native_to_little(data);\n");
                }
                if (sig.ExtendedValueType() == ISignal::EExtendedValueType::Double)
                {
                    os << boost::format("    return data;\n");
                }
                else
                {
                    os << boost::format("    data >>= %1%ull;\n")
                        % sigi._fixed_start_bit_0;
                    if (sig.ExtendedValueType() != ISignal::EExtendedValueType::Float)
                    {
                        os << boost::format("    data &= %1%ull;\n") % sigi._mask;
                        if (sig.ValueType() == ISignal::EValueType::Signed)
                        {
                            os << boost::format(
                                "    if (data & %1%ull)\n"
                                "    {\n"
                                "        data |= %1%ull;\n"
                                "    }\n"
                                "    return data;\n")
                                % sigi._mask_signed;
                        }
                    }
                    os << boost::format("    return data;\n");
                }
            }
            else
            {
                // Alignment::signal_exceeds_64_bit_size_and_signal_does_not_fit_into_64_bit;
                os << boost::format(
                    "    data = *reinterpret_cast<const uint64_t*>(&reinterpret_cast<const uint8_t*>(nbytes)[%1%]);\n")
                    % sigi._byte_pos;
                os << boost::format(
                    "    uint64_t data1 = reinterpret_cast<const uint8_t*>(nbytes)[%1% + 8];\n")
                    % sigi._byte_pos;
                if (sig.ByteOrder() == ISignal::EByteOrder::BigEndian)
                {
                    os << boost::format(
                        "    data = dbcppp_native_to_big(data);\n"
                        "    data &= %1%ull;\n"
                        "    data <<= %2%ull;\n"
                        "    data1 >>= %3%ull;\n"
                        "    data |= data1;\n")
                        % sigi._mask
                        % sigi._fixed_start_bit_0
                        % sigi._fixed_start_bit_1;
                }
                else
                {
                    os << boost::format(
                        "    data = dbcppp_native_to_little(data);\n"
                        "    data >>= %1%ull;\n"
                        "    data1 &= %2%ull;\n"
                        "    data1 <<= %3%ull;\n"
                        "    data |= data1;)\n")
                        % sigi._fixed_start_bit_0
                        % sigi._mask
                        % sigi._fixed_start_bit_1;
                }
                switch (sig.ExtendedValueType())
                {
                case ISignal::EExtendedValueType::Integer:
                    if (sig.ValueType() == ISignal::EValueType::Signed)
                    {
                        os << boost::format(
                            "    if (data & %1%ull)\n"
                            "    {\n"
                            "        data |= %1%ull;\n"
                            "    }\n"
                            "    return data;\n")
                            % sigi._mask_signed;
                        os << boost::format("    return data;\n");
                    }
                    else
                    {
                        os << boost::format("    return data;\n");
                    }
                    break;
                case ISignal::EExtendedValueType::Float:
                    os << boost::format("    return data;\n");
                    break;
                case ISignal::EExtendedValueType::Double:
                    os << boost::format("    return data;\n");
                    break;
                }
            }
            os << boost::format("}\n");
            os << boost::format(
                "double dbcppp_rawToPhys_%1%(uint64_t value)\n"
                "{\n"
                "    return value * %2% + %3%;\n"
                "}\n")
                % (msg.Name() + "_" + std::to_string(msg.Id()) + "_" + sig.Name())
                % sig.Factor()
                % sig.Offset();
        }
    }
    return os;
}
