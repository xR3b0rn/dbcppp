# dbcppp (DBC C++ parser)
A C++ DBC file parser based on `boost.spirit`. This library is designed for decdoing performance.
# Features
* very fast decoding
* verbose parser output in error case
* DBC is editable through C++ interface exported from the library
* de-/serialization of DBC file
* decode functionality for ISO CAN frames (8 data bytes)
* decode functionality for FD CAN frames (64 data bytes) (doesn't work properly yet)
## DBC data types
### Supported
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
### Not supported yet
* sigtype_attr_list
* signal_type_refs
* signal_groups
# Build and install
```
mkdir build
cd build
cmake ..
make -j6
make RunTests
make install
```
## Dependencies
* boost
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
# Decode-function
The signals decode function is using prestored masks and fixed offsets to speed up calculation, therefore the decoding-function should be almost as fast as a code generated decode function would be. The assembly of the `decode8` on its critical path (signed and byte swap must happen) looks similar to this:
```
decode8(unsigned long, unsigned long, unsigned long, unsigned long):
        bswap   rdi
        shr     rdi, cl
        pxor    xmm0, xmm0
        and     rsi, rdi
        mov     rax, rsi
        or      rax, rdx
        test    rsi, rdx
        cmovne  rsi, rax
        cvtsi2sd        xmm0, rsi
        ret
```
Assembly was generated using Compiler Explorer: https://godbolt.org/z/dHRqE4
# Known issues
* decode64 isn't working correctly on LittleEndian-maschine if the ValueType of the Signal is BigEndian
* both decode functions weren't tested on a BigEndian-maschine
# Similar projects
  * [Vector_DBC](https://bitbucket.org/tobylorenz/vector_dbc/src/master/) Does basically the same, the biggest difference is that it uses `bison` instead of `boost::spirit` for grammar parsing
  * [CAN BUS tools in Python 3 (cantools)](https://github.com/eerimoq/cantools) 

