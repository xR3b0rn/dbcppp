# dbcppp
A DBC file parser written in C++.
This library is optimized on speed.

# Build and install
```
cmake .
make
make install
```
# Usage example
```
int main()
{
    Vector::DBC::Network net;
    bool result = parse_dbc("your_test_dbc.dbc", net);
    if (!result)
    {
        std::cout << "DBC parsing failed!" << std::endl;
        return 1;
    }
    auto& signal = net.messages[1234].signals["Signal_name"];
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
