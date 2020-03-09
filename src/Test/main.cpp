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
#include <algorithm>
#include <functional>
#include <ctime>
#include <chrono>
#include <random>
#include <string>

#include <boost/endian/conversion.hpp>

#include "Config.h"

#include "../../include/dbcppp/Network.h"

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
	switch (sig.getExtendedValueType())
	{
	case dbcppp::Signal::ExtendedValueType::Float: return *reinterpret_cast<float*>(&retVal);
	case dbcppp::Signal::ExtendedValueType::Double: return *reinterpret_cast<double*>(&retVal);
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

std::vector<std::string> dbc_to_vec(std::istream& is)
{
	std::vector<std::string> result;
	std::string line;
	while (std::getline(is, line))
	{
		if (line != "")
		{
			result.push_back(line);
		}
	}
	return result;
}

BOOST_AUTO_TEST_CASE(DBCParsing)
{
	std::string dbc_file(TEST_DBC);

	BOOST_TEST_MESSAGE("Testing DBC AST tree for correctness...");

	if (dbc_file != "")
	{
		std::vector<std::string> spec;
		{
			std::ifstream idbc(dbc_file);
			spec = dbc_to_vec(idbc);
		}
		std::ifstream idbc(dbc_file);
		auto net = dbcppp::Network::fromDBC(idbc);
		std::clock_t begin = std::clock();

		BOOST_REQUIRE_MESSAGE(net, "DBC parsing failed!");

		std::stringstream ss;
		net->serializeToStream(ss);
		auto imp = dbc_to_vec(ss);

		for (const auto& line : imp)
		{
			auto iter = std::find(spec.begin(), spec.end(), line);
			BOOST_CHECK_MESSAGE(iter != spec.end(), "line: \"" << line << "\" doesn't match any line in the DBC file!");
			if (iter != spec.end())
			{
				spec.erase(iter);
			}
		}
		{
			std::stringstream ss;
			for (const auto& line : spec)
			{
				ss << line << "\n";
			}
			BOOST_CHECK_MESSAGE(spec.empty(), "Spec isn't empty!\nNot found lines:\n" << ss.str());
		}
	}
	BOOST_TEST_MESSAGE("Done!");
}
BOOST_AUTO_TEST_CASE(Test_decoding)
{
	std::size_t n_tests = 1000000;
	std::size_t max_msg_byte_size = 64;

	BOOST_TEST_MESSAGE("Testing decode-function with " << n_tests << " randomly generated tests...");
	BOOST_TEST_MESSAGE("std::numeric_limits<float>::is_iec559 == " << std::numeric_limits<float>::is_iec559);
	BOOST_TEST_MESSAGE("std::numeric_limits<double>::is_iec559 == " << std::numeric_limits<double>::is_iec559);

	uint32_t seed = static_cast<uint32_t>(time(0));
	std::random_device dev;
	std::default_random_engine rng(seed);
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, -1);

	using namespace dbcppp;

	std::vector<uint8_t> data;
	std::vector<std::size_t> indices;
	for (std::size_t i = 0; i < max_msg_byte_size * 8; i++) indices.push_back(i);
	for (std::size_t i = 0; i < n_tests; i++)
	{
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
		data.clear();
		for (std::size_t j = 0; j < max_msg_byte_size; j++) data.push_back(uint8_t(dist(rng) % 0xFF));
		auto dec_easy = easy_decode(*sig, data);
		auto dec_sig = sig->decode(&data[0]);
		
		std::stringstream ss;
		sig->serializeToStream(ss);
		switch (sig->getExtendedValueType())
		{
		case Signal::ExtendedValueType::Integer: ss << " Intger"; break;
		case Signal::ExtendedValueType::Float: ss << " Float"; break;
		case Signal::ExtendedValueType::Double: ss << " Double"; break;
		}
		// since nan != nan we reintepret_cast to uint64_t before we compare
		BOOST_CHECK_MESSAGE(*reinterpret_cast<uint64_t*>(&dec_easy) == *reinterpret_cast<uint64_t*>(&dec_sig), "No. " + std::to_string(i) + ":\t\"dec_easy == dec_sig\" failed for Signal: " << ss.str());
	}
	BOOST_TEST_MESSAGE("Done!");
}
