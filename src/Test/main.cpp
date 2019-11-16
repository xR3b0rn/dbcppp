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

#include <Vector/DBC.h>
#include <Vector/DBC/Network.h>

#include "../dbcppp/Network.h"
#include "../dbcppp/DBC_Grammar.h"

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F&& func, Args&&... args)
    {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast< TimeT> 
            (std::chrono::steady_clock::now() - start);
        return duration.count();
    }
};
void repeat_n_times(std::size_t n, std::function<void()> func)
{
    for (std::size_t i = 0; i < n; i++)
    {
        func();
    }
}

#include <ctime>

int main()
{
	{
		std::ifstream idbc{"C:/hij/github/dbcppp/src/Test/new_test.dbc"};
		dbcppp::Network net;
		std::clock_t begin = std::clock();
		idbc >> net;
		std::cout << double(std::clock() - begin) / CLOCKS_PER_SEC << std::endl;;



		//for (const auto& msg : net.messages)
		//{
		//	for (const auto& sig : msg.second->signals)
		//	{
		//		std::cout << sig.second->comment << std::endl;
		//	}
		//}
		std::cout << "break" << std::endl;
	}
	{
		std::ifstream idbc{"C:/hij/github/dbcppp/src/Test/new_test.dbc"};
		Vector::DBC::Network net;
		std::clock_t begin = std::clock();
		idbc >> net;
		std::cout << double(std::clock() - begin) / CLOCKS_PER_SEC << std::endl;;
	}

	//bool result = parse_dbc("C:/hij/github/dbcppp/src/Test/test.dbc", net);
	//if (!result)
	//{
	//	std::cout << "DBC parsing failed!" << std::endl;
	//	return 1;
	//}
	//std::array<uint8_t, 8> buff{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}};
	//uint64_t* val = reinterpret_cast<uint64_t*>(&buff[0]);
	//uint64_t raw = 0x8877665544332211;
	//auto& sig = net.messages[2633880062].signals["FMSstandardSWversionSupported"];
	//std::random_device rd;
 //   auto s = rd();
	//std::uniform_int_distribution<uint64_t> dis(0, uint64_t(-1));
 //   {
 //       std::mt19937 gen{s};
 //       auto old_time = measure<>::execution(
 //           repeat_n_times,
 //           99999999,
 //           [&gen, &dis, &sig]()
 //           {
 //               uint64_t val = dis(gen);
 //               volatile uint64_t raw = sig->decode({(uint8_t*)&val, 8});
 //           });
 //       std::cout << "decode: " << old_time << std::endl;
 //   }
 //   {
 //       std::mt19937 gen{s};
 //       auto old_time = measure<>::execution(
 //           repeat_n_times,
 //           99999999,
 //           [&gen, &dis, &sig]()
 //           {
 //               uint64_t val = dis(gen);
 //               volatile uint64_t raw = sig->decode_({(uint8_t*)&val, 8});
 //           });
 //       std::cout << "decode_: " << old_time << std::endl;
 //   }


	//uint64_t ret1 = sig->decode({(uint8_t*)&raw, 8});
	//uint64_t ret2 = sig->decode_({(uint8_t*)&raw, 8});
	//std::vector<uint8_t> vec;
	//vec.resize(8);
	//memcpy((char*)&vec[0], (char*)&raw, 8);
	//uint64_t ret3 = sig->old_decode(vec);

	//std::vector<uint8_t> enc(8, 0);
	//sig->encode({&enc[0], 8}, ret1);

	//std::cout << std::hex << ret1 << std::endl;
}
