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
#include "../dbcppp/MakeSignal.h"

#define BOOST_TEST_MODULE test
#include <boost/test/included/unit_test.hpp>
namespace utf = boost::unit_test;

int64_t easy_decode(const dbcppp::Signal& sig, std::vector<uint8_t> & data)
{
    if (sig.bit_size == 0)
    {
        return 0;
    }
    int64_t retVal = 0;
    if (sig.byte_order == dbcppp::Signal::ByteOrder::BigEndian)
    {
        unsigned int srcBit = sig.start_bit;
        unsigned int dstBit = sig.bit_size - 1;
        for (auto i = 0; i < sig.bit_size; ++i)
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
        unsigned int srcBit = sig.start_bit;
        unsigned int dstBit = 0;
        for (auto i = 0; i < sig.bit_size; ++i)
        {
            if (data[srcBit / 8] & (1 << (srcBit % 8)))
            {
                retVal |= (1ULL << dstBit);
            }
            ++srcBit;
            ++dstBit;
        }
    }
    if (sig.value_type == dbcppp::Signal::ValueType::Signed)
    {
        if (retVal & (1ull << (sig.bit_size - 1)))
        {
            for (auto i = sig.bit_size; i < 64; ++i)
            {
                retVal |= (1ULL << i);
            }
        }
    }
    return retVal;
}
BOOST_AUTO_TEST_CASE(SignalLittleEndian)
{
    auto sig = dbcppp::make_signal(dbcppp::Signal::ByteOrder::LittleEndian, dbcppp::Signal::ValueType::Signed);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, -1);
    std::chrono::milliseconds sum_t_1(0);
    std::chrono::milliseconds sum_t_2(0);
    for (std::size_t i = 0; i < 100000; i++)
    {
        sig->byte_order = dbcppp::Signal::ByteOrder::LittleEndian;
        sig->value_type = dbcppp::Signal::ValueType::Signed;
        sig->start_bit = dis(gen) % 62;
        sig->bit_size = dis(gen) % (64 - sig->start_bit);
        sig->fix_performance_attributes();
        uint64_t data64 = dis(gen);
        uint8_t data1[8];
        std::vector<uint8_t> data2;
        for (std::size_t i = 0; i < 8; i++)
        {
            data1[i] = data64 >> (i * 8);
            data2.push_back(data64 >> (i * 8));
        }
        auto start = std::chrono::system_clock::now().time_since_epoch().count();
        auto raw1 = sig->decode8(data1);
        sum_t_1 += std::chrono::milliseconds((std::chrono::system_clock::now().time_since_epoch().count() - start));
        start = std::chrono::system_clock::now().time_since_epoch().count();
        auto raw2 = easy_decode(*sig, data2);
        sum_t_2 += std::chrono::milliseconds((std::chrono::system_clock::now().time_since_epoch().count() - start));
        BOOST_CHECK_EQUAL(raw1, raw2);
    }
    std::cout << "SignalLittleEndian" << std::endl;
    std::cout << "New method: " << sum_t_1.count() << "ms" << std::endl;
    std::cout << "Easy method: " << sum_t_2.count() << "ms" << std::endl;
    std::cout << "Factor: " << double(sum_t_2.count()) / sum_t_1.count() << std::endl;
}
BOOST_AUTO_TEST_CASE(SignalLittleEndianFd)
{
    auto sig = dbcppp::make_signal(dbcppp::Signal::ByteOrder::LittleEndian, dbcppp::Signal::ValueType::Signed);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, -1);
    std::chrono::milliseconds sum_t_1(0);
    std::chrono::milliseconds sum_t_2(0);
    for (std::size_t i = 0; i < 100000; i++)
    {
        sig->byte_order = dbcppp::Signal::ByteOrder::LittleEndian;
        sig->value_type = dbcppp::Signal::ValueType::Signed;
        sig->start_bit = dis(gen) % 62;
        sig->bit_size = dis(gen) % (64 - sig->start_bit);
        sig->fix_performance_attributes();
        uint8_t data1[64];
        std::vector<uint8_t> data2;
        for (std::size_t j = 0; j < 8; j++)
        {
            uint64_t data64 = dis(gen);
            for (std::size_t i = 0; i < 8; i++)
            {
                data1[j * 8 + i] = (data64 >> (i * 8)) & 0xFFu;
                data2.push_back((data64 >> (i * 8)) & 0xFFu);
            }
        }
        auto start = std::chrono::system_clock::now().time_since_epoch().count();
        auto raw1 = sig->decode64(data1);
        sum_t_1 += std::chrono::milliseconds((std::chrono::system_clock::now().time_since_epoch().count() - start));
        start = std::chrono::system_clock::now().time_since_epoch().count();
        auto raw2 = easy_decode(*sig, data2);
        sum_t_2 += std::chrono::milliseconds((std::chrono::system_clock::now().time_since_epoch().count() - start));
        BOOST_CHECK_EQUAL(raw1, raw2);
    }
    std::cout << "SignalLittleEndianFd" << std::endl;
    std::cout << "New method: " << sum_t_1.count() << "ms" << std::endl;
    std::cout << "Easy method: " << sum_t_2.count() << "ms" << std::endl;
    std::cout << "Factor: " << double(sum_t_2.count()) / sum_t_1.count() << std::endl;
}
BOOST_AUTO_TEST_CASE(SignalBigEndian)
{
    auto sig = dbcppp::make_signal(dbcppp::Signal::ByteOrder::BigEndian, dbcppp::Signal::ValueType::Signed);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(10, -1);
    std::chrono::milliseconds sum_t_1(0);
    std::chrono::milliseconds sum_t_2(0);
    for (std::size_t i = 0; i < 100000; i++)
    {
        sig->byte_order = dbcppp::Signal::ByteOrder::BigEndian;
        sig->value_type = dbcppp::Signal::ValueType::Signed;
        bool done = false;
        bool retry = false;
        while (!done)
        {
            done = true;
            sig->start_bit = dis(gen) % 62;
            sig->bit_size = dis(gen) % (64 - sig->start_bit);
            auto start = sig->start_bit;
            for (std::size_t i = 0; i < sig->bit_size; i++)
            {
                if (start % 8 == 0)
                {
                    start += 15;
                }
                else
                {
                    start--;
                }
                if (start > 63)
                {
                    done = false;
                    break;
                }
            }
        }
        sig->fix_performance_attributes();
        uint64_t data64 = dis(gen);
        uint8_t data1[8];
        std::vector<uint8_t> data2;
        for (std::size_t i = 0; i < 8; i++)
        {
            data1[i] = data64 >> (i * 8);
            data2.push_back(data64 >> (i * 8));
        }
        auto start = std::chrono::system_clock::now().time_since_epoch().count();
        auto raw1 = sig->decode8(data1);
        sum_t_1 += std::chrono::milliseconds((std::chrono::system_clock::now().time_since_epoch().count() - start));
        start = std::chrono::system_clock::now().time_since_epoch().count();
        auto raw2 = easy_decode(*sig, data2);
        sum_t_2 += std::chrono::milliseconds((std::chrono::system_clock::now().time_since_epoch().count() - start));
        BOOST_CHECK_EQUAL(raw1, raw2);
    }
    std::cout << "SignalBigEndian" << std::endl;
    std::cout << "New method: " << sum_t_1.count() << "ms" << std::endl;
    std::cout << "Easy method: " << sum_t_2.count() << "ms" << std::endl;
    std::cout << "Factor: " << double(sum_t_2.count()) / sum_t_1.count() << std::endl;
}
BOOST_AUTO_TEST_CASE(SignalBigEndianFd)
{
    auto sig = dbcppp::make_signal(dbcppp::Signal::ByteOrder::BigEndian, dbcppp::Signal::ValueType::Signed);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(10, -1);
    std::chrono::milliseconds sum_t_1(0);
    std::chrono::milliseconds sum_t_2(0);
    for (std::size_t i = 0; i < 100000; i++)
    {
        sig->byte_order = dbcppp::Signal::ByteOrder::BigEndian;
        sig->value_type = dbcppp::Signal::ValueType::Signed;
        bool done = false;
        bool retry = false;
        while (!done)
        {
            done = true;
            sig->start_bit = dis(gen) % 62;
            sig->bit_size = dis(gen) % (64 - sig->start_bit);
            auto start = sig->start_bit;
            for (std::size_t i = 0; i < sig->bit_size; i++)
            {
                if (start % 8 == 0)
                {
                    start += 15;
                }
                else
                {
                    start--;
                }
                if (start > 63)
                {
                    done = false;
                    break;
                }
            }
        }
        sig->fix_performance_attributes();
        uint64_t data64 = dis(gen);
        uint8_t data1[64];
        std::vector<uint8_t> data2;
        for (std::size_t j = 0; j < 8; j++)
        {
            uint64_t data64 = dis(gen);
            for (std::size_t i = 0; i < 8; i++)
            {
                data1[j * 8 + i] = (data64 >> (i * 8)) & 0xFFu;
                data2.push_back((data64 >> (i * 8)) & 0xFFu);
            }
        }
        auto start = std::chrono::system_clock::now().time_since_epoch().count();
        auto raw1 = sig->decode64(data1);
        sum_t_1 += std::chrono::milliseconds((std::chrono::system_clock::now().time_since_epoch().count() - start));
        start = std::chrono::system_clock::now().time_since_epoch().count();
        auto raw2 = easy_decode(*sig, data2);
        sum_t_2 += std::chrono::milliseconds((std::chrono::system_clock::now().time_since_epoch().count() - start));
        BOOST_CHECK_EQUAL(raw1, raw2);
    }
    std::cout << "SignalBigEndianFd" << std::endl;
    std::cout << "New method: " << sum_t_1.count() << "ms" << std::endl;
    std::cout << "Easy method: " << sum_t_2.count() << "ms" << std::endl;
    std::cout << "Factor: " << double(sum_t_2.count()) / sum_t_1.count() << std::endl;
}

//int main()
//{
//	std::cout << "test" << std::endl;
//}
