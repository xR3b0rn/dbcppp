# dbcppp (DBC C++ parser)
A DBC file parser written in C++ and based on `boost.spirit`. This library is designed for performance.

# Build and install
```
mkdir build
cd build
cmake ..
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
    std::unique_ptr<const dbcppp::Network> net = dbcppp::Network::fromDBC(dbc_file);
    if (net)
    {
        can_frame frame;
        canfd_frame fd_frame;
        while (1)
        {
            receive_can_frame_from_somewhere(&frame);
            receive_canfd_frame_from_somewhere(&fd_frame);
            auto& msg = net->getMessageById(frame.id);
            std::cout << "Received message: " << msg->getName() << std::endl;
            for (auto* signal : msg->getSignals())
            {
                // either this for standard CAN frames
                double raw = signal->decode8(frame.data);
                // or this for FD CAN frames
                // double raw = signal->decode64(fd_frame.data);
                std::cout << "\t" << signal->name << "=" << signal->raw_to_phys(raw) << std::endl;
            }
        }
    }
}

```
# DBC data types
## Supported
* version
* new_symbols
* bit_timing
* nodes
* value_tables
* messages
* message_transmitters
* environment_variables
* environment_variables_data
* signal_types
* comments
* attribute_definitions
* attribute_defaults
* attribute_values
* value_descriptions
* signal_extended_value_type_list
## Not supported yet
* sigtype_attr_list
* signal_type_refs
* signal_groups
# Decode-function
The signals decode function is using prestored masks and fixed offsets to speed up calculation. The assembly of the `decode8` on its critical path (signed and byte swap must happen) looks similar to this:
```
template_decode8(Signal const*, void const*):
        mov     rax, QWORD PTR [rsi]
        mov     rcx, QWORD PTR [rdi]
        pxor    xmm0, xmm0
        mov     rdx, QWORD PTR [rdi+16]
        bswap   rax
        shr     rax, cl
        and     rax, QWORD PTR [rdi+8]
        and     rdx, rax
        neg     rdx
        or      rax, rdx
        cvtsi2sd        xmm0, rax
        ret
```
Binary was generated using Compiler Explorer: https://godbolt.org/z/8YFjok
# Known issues
* decode64 isn't working correctly on LittleEndian-maschine if the ValueType of the Signal is BigEndian-signal
* both decode functions weren't tested on a BigEndian-maschine
# Similar projects
  * [Vector_DBC](https://bitbucket.org/tobylorenz/vector_dbc/src/master/) Does basically the same, the biggest difference is that it uses `bison` instead of `boost::spirit` for grammar parsing
  * [CAN BUS tools in Python 3 (cantools)](https://github.com/eerimoq/cantools) 

