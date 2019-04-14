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
#include <iostream>
#include <Vector/DBC/Spirit.h>
#include <Vector/DBC/Network.h>

int main()
{
	Vector::DBC::Network net;
	bool result = parse_dbc("your_test_dbc.dbc", net);
	if (!result)
	{
		std::cout << "DBC parsing failed!" << std::endl;
		return 1;
	}
	std::array<uint8_t, 8> buff{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}};
	uint64_t* val = reinterpret_cast<uint64_t*>(&buff[0]);
	uint64_t raw = 0x8877665544332211;
	auto& sig = net.messages[2633880062].signals["FMSstandardSWversionSupported"];
	uint64_t ret = sig.decode({(uint8_t*)&raw, 8});

	std::vector<uint8_t> enc(8, 0);
	sig.encode({&enc[0], 8}, ret);

	std::cout << std::hex << ret << std::endl;
}
