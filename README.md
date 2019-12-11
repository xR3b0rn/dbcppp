# dbcppp (DBC C++ parser)
A DBC file parser written in C++ and based on `boost.spirit`. This library designed for performance.

# Build and install
```
cmake .
make
make install
```
# Usage example
```C++
#include <fstream>
#include <dbcppp/Network.h>
int main()
{
    std::ifstream dbc_file{"your_dbc.dbc"};
    dbcppp::Network net;
    if (!(dbc_file >> net)
    {
        std::cout << "DBC parsing failed!" << std::endl;
        return 1;
    }
    can_frame frame;
    while (1)
    {
        receive_can_frame_from_somewhere(&frame);
        auto& msg = net.messages[frame.id];
        std::cout << "Received message: " << msg.name << std::endl;
        for (auto& signal : msg.signals)
        {
            uint64_t data =
                (uint64_t)frame.data[0] | ((uint64_t)frame.data[1] << 8) | ((uint64_t)frame.data[2] << 16)
             | ((uint64_t)frame.data[3] << 24) | ((uint64_t)frame.data[4] << 32) | ((uint64_t)frame.data[5] << 40)
             | ((uint64_t)frame.data[6] << 48) | ((uint64_t)frame.data[7] << 56);
            int64_t raw = signal.decode(&frame.data);
            std::cout << "\t" << signal.name << "=" << signal.raw_to_phys(raw) << std::endl;
        }
    }
}

```
### Similar projects
  * [Vector_DBC](https://bitbucket.org/tobylorenz/vector_dbc/src/master/) Does basically the same, the biggest difference is that it uses `bison` instead of `boost::spirit` for grammar parsing
  * [CAN BUS tools in Python 3 (cantools)](https://github.com/eerimoq/cantools) 

