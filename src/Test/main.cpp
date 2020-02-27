/*
    This file is part of the dbcppp (DBC C++ parser) program.
    Copyright (C) 2019 Julian Hindelang

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <array>
#include <fstream>
#include <iostream>
#include <iostream>
#include <functional>
#include <ctime>
#include <chrono>
#include <random>

#include <boost/endian/conversion.hpp>

#include "../dbcppp/Network.h"
#include "../dbcppp/DBC_Grammar.h"
#include "../dbcppp/SignalImpl.h"

#define BOOST_TEST_MODULE test
#include <boost/test/included/unit_test.hpp>
namespace utf = boost::unit_test;

double easy_decode(dbcppp::Signal& sig, std::vector<uint8_t>& data)
{
    if (sig.getBitSize() == 0)
    {
        return 0;
    }
    uint64_t retVal = 0;
    if (sig.getByteOrder() == dbcppp::Signal::ByteOrder::BigEndian)
    {
        auto srcBit = sig.getStartBit();
        auto dstBit = sig.getBitSize() - 1;
        for (auto i = 0; i < sig.getBitSize(); ++i)
        {
            if (data[srcBit / 8] & (1ull << (srcBit % 8)))
            {
                retVal |= (1ULL << dstBit);
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
                retVal |= (1ULL << dstBit);
            }
            ++srcBit;
            ++dstBit;
        }
    }
    if (sig.getValueType() == dbcppp::Signal::ValueType::Signed)
    {
        if (retVal & (1ull << (sig.getBitSize() - 1)))
        {
            for (auto i = sig.getBitSize(); i < 64; ++i)
            {
                retVal |= (1ULL << i);
            }
        }
        return double(*reinterpret_cast<int64_t*>(&retVal));
    }
    return double(retVal);
}
BOOST_AUTO_TEST_CASE(Test_Parsing)
{
    auto net = dbcppp::Network::create();
    // TODO: create test DBC-file
}
BOOST_AUTO_TEST_CASE(Test_Decoding8)
{
    std::cout << "Testing decode8-function with 99999 randomly generated tests..." << std::endl;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, -1);

    using namespace dbcppp;
    SignalImpl sig(Signal::ByteOrder::BigEndian, Signal::ValueType::Signed, 56, 5, 8);
    std::vector<uint8_t> data;
    for (std::size_t i = 0; i < 99999; i++)
    {
        auto rnd_byte_order = dist(rng) % 2 == 0 ? Signal::ByteOrder::LittleEndian : Signal::ByteOrder::BigEndian;
        auto rnd_value_type = dist(rng) % 2 == 0 ? Signal::ValueType::Unsigned : Signal::ValueType::Signed;
        auto rnd_start_bit = dist(rng) % 54;
        do
        {
            auto rnd_bit_size = dist(rng) % (64 - rnd_start_bit);
            sig = SignalImpl(rnd_byte_order, rnd_value_type, rnd_bit_size, rnd_start_bit, 8);
        } while (sig.getError() != Signal::ErrorCode::NoError);

        data.clear();
        for (std::size_t j = 0; j < 8; j++)
        {
            data.push_back(uint8_t(dist(rng) % 0xFF));
        }
        auto dec_easy = easy_decode(sig, data);
        auto dec_sig = sig.decode8(&data[0]);

        BOOST_CHECK(dec_easy == dec_sig);
    }
}
BOOST_AUTO_TEST_CASE(Test_Decoding64)
{
    std::cout << "Testing decode64-function with 99999 randomly generated tests..." << std::endl;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, -1);

    using namespace dbcppp;
    SignalImpl sig(Signal::ByteOrder::BigEndian, Signal::ValueType::Signed, 56, 5, 8);
    std::vector<uint8_t> data;
    for (std::size_t i = 0; i < 99999; i++)
    {
        auto rnd_byte_order = dist(rng) % 2 == 0 ? Signal::ByteOrder::LittleEndian : Signal::ByteOrder::BigEndian;
        auto rnd_value_type = dist(rng) % 2 == 0 ? Signal::ValueType::Unsigned : Signal::ValueType::Signed;
        auto rnd_start_bit = dist(rng) % 54;
        do
        {
            auto rnd_bit_size = dist(rng) % (64 - rnd_start_bit);
            sig = SignalImpl(rnd_byte_order, rnd_value_type, rnd_bit_size, rnd_start_bit, 8);
        } while (sig.getError() != Signal::ErrorCode::NoError);

        data.clear();
        for (std::size_t j = 0; j < 64; j++)
        {
            data.push_back(uint8_t(dist(rng) % 0xFF));
        }
        auto dec_easy = easy_decode(sig, data);
        auto dec_sig = sig.decode8(&data[0]);

        BOOST_CHECK(dec_easy == dec_sig);
    }
}
