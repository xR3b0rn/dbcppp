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
#include <string>

#include <boost/endian/conversion.hpp>


#include "../dbcppp/DBC_Grammar.h"
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
/*
BOOST_AUTO_TEST_CASE(Test_Parsing)
{
    // TODO: create test DBC-file
}*/

BOOST_AUTO_TEST_CASE(DBCParsing)
{
    std::ifstream idbc{"C:/hij/github/dbcppp/src/Test/TeslaModel3.dbc"};
    auto net = dbcppp::Network::create();
    std::clock_t begin = std::clock();

    if (!(idbc >> *net))
    {
        std::cout << "DBC parsing failed!" << std::endl;
        //return 1;
    }
    std::cout << double(std::clock() - begin) / CLOCKS_PER_SEC << std::endl;;

    std::cout << "Network" << std::endl;
    std::cout
            << "version: " << net->getVersion() << "\n"
            << "bit timing " << net->getBitTiming().getBaudrate()
            << std::endl;

    std::cout << " signal_extended_value_types (" << net->getSignalExtendedValues().size() << ")" << std::endl;
    for (auto element : net->getSignalExtendedValues())
    {
        std::cout << "      " << element->getMessageId() << " " << element->getSignalName() << " " << element->getValue() << std::endl;
    }

    for (auto& message : net->getMessages())
    {
        std::cout << message.first
                  << " name: " << message.second->getName()
                  << std::endl;

        for (auto& signal : message.second->getSignals())
        {
            for(auto& valtype : net->getSignalExtendedValues())
            {
                if(valtype->getMessageId() == message.first && signal.second->getName() == valtype->getSignalName())
                {
                    std::cout << valtype->getMessageId() <<   " "  << valtype->getSignalName() << " " << valtype->getValue()  << std::endl;
                }
                else
                {
                    std::cout << "default" << std::endl;
                }
            }
        }

        for (auto& signal : message.second->getSignals())
        {
            std::cout << "     "
                      << signal.first
                      << " name: " << signal.second->getName()
                      << ", start_bit: " << signal.second->getStartBit()
                      << ", bit_size : " << signal.second->getBitSize()
                      << ", factor " << signal.second->getFactor()
                      << ", offset " << signal.second->getOffset()
                      << ", min " << signal.second->getMinimum()
                      << ", max " << signal.second->getMaximum()
                      << ", comment" << signal.second->getComment()
                      << std::endl;

            std::cout << "Attributes values" ;
            for (auto& element : signal.second->getAttributeValues()) {
                std::cout << element.first << std::endl;
            }
        }
    }
}
BOOST_AUTO_TEST_CASE(Test_Decoding8)
{
    std::size_t n_tests = 99999;

    BOOST_TEST_MESSAGE("Testing decode8-function with " << n_tests << " randomly generated tests...");

    uint32_t seed = static_cast<uint32_t>(time(0));
    std::random_device dev;
    std::default_random_engine rng(seed);
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, -1);

    using namespace dbcppp;
    SignalImpl sig(Signal::ByteOrder::BigEndian, Signal::ValueType::Signed, 56, 5, 8);
    std::vector<uint8_t> data;
    for (std::size_t i = 0; i < n_tests; i++)
    {
        auto rnd_byte_order = dist(rng) % 2 == 0 ? Signal::ByteOrder::LittleEndian : Signal::ByteOrder::BigEndian;
        auto rnd_value_type = dist(rng) % 2 == 0 ? Signal::ValueType::Unsigned : Signal::ValueType::Signed;
        auto rnd_start_bit = dist(rng) % 64;
        while (rnd_start_bit == 56)
        {
            rnd_start_bit = dist(rng) % 64;
        }
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
        
        BOOST_REQUIRE_MESSAGE(dec_easy == dec_sig,
            ("seed=" + std::to_string(seed)
            + " i=" + std::to_string(i)
            + " dec_easy=" + std::to_string(dec_easy)
            + " dec_sig=" + std::to_string(dec_sig)));
    }
}
BOOST_AUTO_TEST_CASE(Test_Decoding64)
{
    std::size_t n_tests = 99999;

    BOOST_TEST_MESSAGE("Testing decode64-function with " << n_tests << " randomly generated tests...");

    uint32_t seed = static_cast<uint32_t>(time(0));
    std::random_device dev;
    std::default_random_engine rng(seed);
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, -1);

    using namespace dbcppp;
    SignalImpl sig(Signal::ByteOrder::BigEndian, Signal::ValueType::Signed, 56, 5, 8);
    std::vector<uint8_t> data;
    for (std::size_t i = 0; i < n_tests; i++)
    {
        auto rnd_byte_order = dist(rng) % 2 == 0 ? Signal::ByteOrder::LittleEndian : Signal::ByteOrder::BigEndian;
        auto rnd_value_type = dist(rng) % 2 == 0 ? Signal::ValueType::Unsigned : Signal::ValueType::Signed;
        auto rnd_start_bit = dist(rng) % 64;
        while (rnd_start_bit == 56)
        {
            rnd_start_bit = dist(rng) % 64;
        }
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
        
        BOOST_REQUIRE_MESSAGE(dec_easy == dec_sig,
            ("seed=" + std::to_string(seed)
            + " i=" + std::to_string(i)
            + " dec_easy=" + std::to_string(dec_easy)
            + " dec_sig=" + std::to_string(dec_sig)));
    }
}
