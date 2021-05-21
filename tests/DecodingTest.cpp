#include <array>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <ctime>
#include <chrono>
#include <random>
#include <string>
#include <iomanip>

#include "../include/dbcppp/Network2Functions.h"
#include "../include/dbcppp/CApi.h"
#include "../include/dbcppp/Network.h"

#include "Catch2.h"

auto generate_random_signal(
      std::size_t max_msg_byte_size
    , std::default_random_engine& rng)
{
    using namespace dbcppp;

    std::vector<std::size_t> indices;
    for (std::size_t i = 0; i < max_msg_byte_size * 8; i++) indices.push_back(i);

    std::uniform_int_distribution<std::mt19937::result_type> dist(0, -1);
    std::unique_ptr<ISignal> sig;
    auto rnd_msg_byte_size = dist(rng) % max_msg_byte_size + 1;
    auto rnd_byte_order = dist(rng) % 2 == 0 ? ISignal::EByteOrder::LittleEndian : ISignal::EByteOrder::BigEndian;
    auto rnd_value_type = dist(rng) % 2 == 0 ? ISignal::EValueType::Unsigned : ISignal::EValueType::Signed;
    auto rnd_bit_size = dist(rng) % (((rnd_msg_byte_size > 8) ? 8 : rnd_msg_byte_size) * 8) + 1;
    ISignal::EExtendedValueType rnd_extended_value_type = ISignal::EExtendedValueType::Integer;
    auto rnd_evt = dist(rng) % 3;
    if (rnd_msg_byte_size >= 4)
    {
        if (std::numeric_limits<float>::is_iec559 && rnd_evt == 1)
        {
            rnd_extended_value_type = ISignal::EExtendedValueType::Float;
            rnd_bit_size = 32;
        }
        else if (std::numeric_limits<double>::is_iec559 && rnd_msg_byte_size >= 8 && rnd_evt == 2)
        {
            rnd_extended_value_type = ISignal::EExtendedValueType::Double;
            rnd_bit_size = 64;
        }
    }
    if (rnd_byte_order == ISignal::EByteOrder::LittleEndian)
    {
        uint64_t rnd_start_bit = 0;
        if ((rnd_msg_byte_size * 8 - rnd_bit_size) != 0)
        {
            rnd_start_bit = dist(rng) % (rnd_msg_byte_size * 8 - rnd_bit_size);
        }
        sig = ISignal::Create(rnd_msg_byte_size, "Signal", ISignal::EMultiplexer::NoMux, 0, rnd_start_bit, rnd_bit_size,
            rnd_byte_order, rnd_value_type, 1.0, 0.0, 0.0, 0.0, "", {}, {}, {}, "", rnd_extended_value_type, {});
    }
    else
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(indices.begin(), indices.end(), g);
        for (auto rnd_start_bit : indices)
        {
            sig = ISignal::Create(rnd_msg_byte_size, "Signal", ISignal::EMultiplexer::NoMux, 0, rnd_start_bit, rnd_bit_size,
                rnd_byte_order, rnd_value_type, 1.0, 0.0, 0.0, 0.0, "", {}, {}, {}, "", rnd_extended_value_type, {});
            if (sig->Error(ISignal::EErrorCode::NoError))
            {
                break;
            }
        }
    }
    return std::move(sig);
}
auto generate_random_data(
      std::size_t max_msg_byte_size
    , std::default_random_engine& rng)
{
    std::vector<uint8_t> result;
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, -1);
    for (std::size_t j = 0; j < max_msg_byte_size; j++) result.push_back(uint8_t(dist(rng) & 0xFF));
    return result;
}

uint64_t easy_decode(dbcppp::ISignal& sig, std::vector<uint8_t>& data)
{
    if (sig.BitSize() == 0)
    {
        return 0;
    }
    uint64_t result = 0;
    if (sig.ByteOrder() == dbcppp::ISignal::EByteOrder::BigEndian)
    {
        auto srcBit = sig.StartBit();
        auto dstBit = sig.BitSize() - 1;
        for (auto i = 0; i < sig.BitSize(); i++)
        {
            if (data[srcBit / 8] & (1ull << (srcBit % 8)))
            {
                result |= (1ULL << dstBit);
            }
            if ((srcBit % 8) == 0)
            {
                srcBit += 15;
            }
            else
            {
                --srcBit;
            }
            --dstBit;
        }
    }
    else
    {
        auto srcBit = sig.StartBit();
        auto dstBit = 0;
        for (auto i = 0; i < sig.BitSize(); i++)
        {
            if (data[srcBit / 8] & (1 << (srcBit % 8)))
            {
                result |= (1ULL << dstBit);
            }
            ++srcBit;
            ++dstBit;
        }
    }
    switch (sig.ExtendedValueType())
    {
    case dbcppp::ISignal::EExtendedValueType::Float: return result;
    case dbcppp::ISignal::EExtendedValueType::Double: return result;
    }
    if (sig.ValueType() == dbcppp::ISignal::EValueType::Signed)
    {
        if (result & (1ull << (sig.BitSize() - 1)))
        {
            for (auto i = sig.BitSize(); i < 64; ++i)
            {
                result |= (1ULL << i);
            }
        }
        return result;
    }
    return result;
}
TEST_CASE("Decoding")
{
    using namespace dbcppp;

    std::size_t n_tests = 10000;
    std::size_t max_msg_byte_size = 64;

    //BOOST_TEST_MESSAGE("Testing decode-function with " << n_tests << " randomly generated tests...");
    REQUIRE(std::numeric_limits<float>::is_iec559);
    REQUIRE(std::numeric_limits<double>::is_iec559);

    uint32_t seed = static_cast<uint32_t>(time(0));
    std::random_device dev;
    std::default_random_engine rng(seed);
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, -1);

    for (std::size_t i = 0; i < n_tests; i++)
    {
        auto sig = generate_random_signal(max_msg_byte_size, rng);
        auto data = generate_random_data(max_msg_byte_size, rng);
        auto dec_easy = easy_decode(*sig, data);
        auto dec_sig = sig->Decode(&data[0]);
        
        std::stringstream ss;
        ss << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
        {
            using namespace dbcppp::Network2DBC;
            ss << *sig;
        }
        std::string evt = "";
        switch (sig->ExtendedValueType())
        {
        case ISignal::EExtendedValueType::Integer: ss << " Intger"; evt = "Integer"; break;
        case ISignal::EExtendedValueType::Float: ss << " Float"; evt = "Float"; break;
        case ISignal::EExtendedValueType::Double: ss << " Double"; evt = "Double"; break;
        }
        // since nan != nan we reintepret_cast to uint64_t before we compare
        std::string error = "No. " + std::to_string(i) + ":\t\"dec_easy == dec_sig\" failed for Signal: " + ss.str();
        REQUIRE(*reinterpret_cast<uint64_t*>(&dec_easy) == *reinterpret_cast<uint64_t*>(&dec_sig));
    }
    //BOOST_TEST_MESSAGE("Done!");
}