
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

#include "../../include/dbcppp/Network2DBC.h"
#include "../../include/dbcppp/CApi.h"
#include "../../include/dbcppp/Network.h"

#include <boost/test/unit_test.hpp>
namespace utf = boost::unit_test;

auto generate_random_signal(
      std::size_t max_msg_byte_size
    , std::default_random_engine& rng)
{
    using namespace dbcppp;

    std::vector<std::size_t> indices;
    for (std::size_t i = 0; i < max_msg_byte_size * 8; i++) indices.push_back(i);

    std::uniform_int_distribution<std::mt19937::result_type> dist(0, -1);
    std::unique_ptr<Signal> sig;
    auto rnd_msg_byte_size = dist(rng) % max_msg_byte_size + 1;
    auto rnd_byte_order = dist(rng) % 2 == 0 ? Signal::ByteOrder::LittleEndian : Signal::ByteOrder::BigEndian;
    auto rnd_value_type = dist(rng) % 2 == 0 ? Signal::ValueType::Unsigned : Signal::ValueType::Signed;
    auto rnd_bit_size = dist(rng) % (((rnd_msg_byte_size > 8) ? 8 : rnd_msg_byte_size) * 8) + 1;
    Signal::ExtendedValueType rnd_extended_value_type = Signal::ExtendedValueType::Integer;
    auto rnd_evt = dist(rng) % 3;
    if (rnd_msg_byte_size >= 4)
    {
        if (std::numeric_limits<float>::is_iec559 && rnd_evt == 1)
        {
            rnd_extended_value_type = Signal::ExtendedValueType::Float;
            rnd_bit_size = 32;
        }
        else if (std::numeric_limits<double>::is_iec559 && rnd_msg_byte_size >= 8 && rnd_evt == 2)
        {
            rnd_extended_value_type = Signal::ExtendedValueType::Double;
            rnd_bit_size = 64;
        }
    }
    if (rnd_byte_order == Signal::ByteOrder::LittleEndian)
    {
        uint64_t rnd_start_bit = 0;
        if ((rnd_msg_byte_size * 8 - rnd_bit_size) != 0)
        {
            rnd_start_bit = dist(rng) % (rnd_msg_byte_size * 8 - rnd_bit_size);
        }
        sig = Signal::create(rnd_msg_byte_size, "Signal", Signal::Multiplexer::NoMux, 0, rnd_start_bit, rnd_bit_size,
            rnd_byte_order, rnd_value_type, 1.0, 0.0, 0.0, 0.0, "", {}, {}, {}, "", rnd_extended_value_type);
    }
    else
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(indices.begin(), indices.end(), g);
        for (auto rnd_start_bit : indices)
        {
            sig = Signal::create(rnd_msg_byte_size, "Signal", Signal::Multiplexer::NoMux, 0, rnd_start_bit, rnd_bit_size,
                rnd_byte_order, rnd_value_type, 1.0, 0.0, 0.0, 0.0, "", {}, {}, {}, "", rnd_extended_value_type);
            if (sig)
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

double easy_decode(dbcppp::Signal& sig, std::vector<uint8_t>& data)
{
    if (sig.getBitSize() == 0)
    {
        return 0;
    }
    union
    {
        uint64_t ui;
        int64_t i;
        float f;
        double d;
    } retVal{0};
    if (sig.getByteOrder() == dbcppp::Signal::ByteOrder::BigEndian)
    {
        auto srcBit = sig.getStartBit();
        auto dstBit = sig.getBitSize() - 1;
        for (auto i = 0; i < sig.getBitSize(); ++i)
        {
            if (data[srcBit / 8] & (1ull << (srcBit % 8)))
            {
                retVal.ui |= (1ULL << dstBit);
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
        auto srcBit = sig.getStartBit();
        auto dstBit = 0;
        for (auto i = 0; i < sig.getBitSize(); ++i)
        {
            if (data[srcBit / 8] & (1 << (srcBit % 8)))
            {
                retVal.ui |= (1ULL << dstBit);
            }
            ++srcBit;
            ++dstBit;
        }
    }
    switch (sig.getExtendedValueType())
    {
    case dbcppp::Signal::ExtendedValueType::Float: return retVal.f;
    case dbcppp::Signal::ExtendedValueType::Double: return retVal.d;
    }
    if (sig.getValueType() == dbcppp::Signal::ValueType::Signed)
    {
        if (retVal.ui & (1ull << (sig.getBitSize() - 1)))
        {
            for (auto i = sig.getBitSize(); i < 64; ++i)
            {
                retVal.ui |= (1ULL << i);
            }
        }
        return double(retVal.i);
    }
    return double(retVal.ui);
}
BOOST_AUTO_TEST_CASE(Decoding)
{
    using namespace dbcppp;

    std::size_t n_tests = 1000000;
    std::size_t max_msg_byte_size = 64;

    BOOST_TEST_MESSAGE("Testing decode-function with " << n_tests << " randomly generated tests...");
    BOOST_CHECK(std::numeric_limits<float>::is_iec559);
    BOOST_CHECK(std::numeric_limits<double>::is_iec559);

    uint32_t seed = static_cast<uint32_t>(time(0));
    std::random_device dev;
    std::default_random_engine rng(seed);
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, -1);

    for (std::size_t i = 0; i < n_tests; i++)
    {
        auto sig = generate_random_signal(max_msg_byte_size, rng);
        auto data = generate_random_data(max_msg_byte_size, rng);
        auto dec_easy = easy_decode(*sig, data);
        auto dec_sig = sig->decode(&data[0]);
        
        std::stringstream ss;
        ss << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
        {
            using namespace dbcppp::Network2DBC;
            ss << *sig;
        }
        std::string evt = "";
        switch (sig->getExtendedValueType())
        {
        case Signal::ExtendedValueType::Integer: ss << " Intger"; evt = "Integer"; break;
        case Signal::ExtendedValueType::Float: ss << " Float"; evt = "Float"; break;
        case Signal::ExtendedValueType::Double: ss << " Double"; evt = "Double"; break;
        }
        // since nan != nan we reintepret_cast to uint64_t before we compare
        BOOST_CHECK_MESSAGE(*reinterpret_cast<uint64_t*>(&dec_easy) == *reinterpret_cast<uint64_t*>(&dec_sig), "No. " + std::to_string(i) + ":\t\"dec_easy == dec_sig\" failed for Signal: " << ss.str());
    }
    BOOST_TEST_MESSAGE("Done!");
}
