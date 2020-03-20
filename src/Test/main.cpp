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
   // std::string lane_host(Core_Lanes_Host_protocol);
    //std::string objects(Core_Objects_protocol);
    BOOST_TEST_MESSAGE("Testing DBC AST tree for correctness...");

    std::ifstream lane_host(Core_Lanes_Host_protocol);
    std::ifstream object(Core_Objects_protocol);

    auto net =  dbcppp::Network::fromDBC(lane_host);
    net = dbcppp::Network::fromDBC(object, std::move(net) );
    std::clock_t begin = std::clock();

    net->forEachMessage
            (
                    [&](const dbcppp::Message& m)
                    {

                        std::cout << m.getName() << std::endl;
                        std::cout << "for Each Signal by Start Bit" << std::endl;
                        m.forEachSignalbyStartBit
                                (
                                        [&](const dbcppp::Signal& s)
                                        {
                                            std::cout << " Name: " << s.getName() << ", Start bit: " << s.getStartBit() << ", Max: "  << s.getMaximum() << ", Bitsize: " << s.getBitSize() << ", Comment: " << s.getComment() << std::endl;
                                        }
                                );

                        std::cout << "for Each Signal" << std::endl;
                        m.forEachSignal
                                (
                                        [&](const dbcppp::Signal& s)
                                        {
                                            std::cout << " Name: " << s.getName() << ", Start bit: " << s.getStartBit() << ", Max: "  << s.getMaximum() << ", Bitsize: " << s.getBitSize() << ", Comment: " << s.getComment() << std::endl;
                                        }
                                );
                    }
            );

  BOOST_TEST_MESSAGE("Done!");
}