
#include <iostream>
#include <Vector/DBC/Spirit.h>
#include <Vector/DBC/Network.h>
#include <array>

int main()
{
	Vector::DBC::Network net;
	bool result = parse_dbc("C:/Users/jh/source/repos/boost_spirit/boost_spirit/test.dbc", net);
	if (result)
	{
		std::cout << "Successfully parsed the dbc!" << std::endl;
	}
	std::array<uint8_t, 8> buff{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}};
	uint64_t* val = reinterpret_cast<uint64_t*>(&buff[0]);
	uint64_t raw = 0x8877665544332211;
	auto& sig = net.messages[2633880062].signals["FMSstandardSWversionSupported"];
	uint64_t ret = sig.decode({(uint8_t*)&raw, 8});

	std::vector<uint8_t> enc(8, 0);
	sig.encode(enc, ret);

	std::cout << std::hex << ret << std::endl;
}
