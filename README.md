# dbcppp (DBC C++ parser)
A DBC file parser written in C++ and based on `boost.spirit`. This library is performance optimized.

# Build and install
```
cmake .
make
make install
```
# Usage example
```
#include <fstream>
#include <Vector/DBC/Spirit.h>
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
            int64_t raw = signal.decode({&frame.data, frame.dlc});
            std::cout << "\t" << signal.name << "=" << signal.physicalToRawValue(raw) << std::endl;
        }
    }
    
}

```
