[![Build Status](https://travis-ci.org/xR3b0rn/dbcppp.svg?branch=master)](https://travis-ci.org/xR3b0rn/dbcppp)
# dbcppp
A C/C++ DBC file parser based on `boost.spirit`. This library is designed for decoding performance.
# Features
* very fast decoding
* verbose parser output in error case
* DBC is editable through C/C++ interface exported from the library
* read DBC file
* decode functionality for frames of arbitrarily byte length
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
* [Examples](https://github.com/xR3b0rn/dbcppp/tree/master/src/Examples)
* `C++`
```C++
#include <fstream>
#include <dbcppp/Network.h>
int main()
{
    std::ifstream dbc_file{"your_dbc.dbc"};
    std::unique_ptr<dbcppp::Network> net = dbcppp::Network::fromDBC(dbc_file);
    if (net)
    {
        can_frame frame;
        while (1)
        {
            receive_can_frame_from_somewhere(&frame);
            const Message* msg = net->getMessageById(frame.id);
            if (msg)
            {
                std::cout << "Received message: " << msg->getName() << std::endl;
                msg->forEachSignal(
                    [&](const Signal& signal)
                    {
                        double raw = signal.decode(frame.data);
                        std::cout << "\t" << signal.getName() << "=" << signal.rawToPhys(raw) << std::endl;
                    });
            }
        }
    }
}
```
* `C`
```C
#include <stdio.h>
#include <dbcppp/CApi.h>
int main()
{
    const dbcppp_Nework* net = dbcppp_NetworkLoadDBCFromFile("your_dbc.dbc");
    if (net)
    {
        can_frame frame;
        while (1)
        {
            receive_can_frame_from_somewhere(&frame);
            const dbcppp_Message* msg = dbcppp_NetworkGetMessageById(net, frame.id);
            if (msg)
            {
                printf("Received message: %s\n", dbcppp_MessageGetName(msg));
                void print_signal_data(const dbcppp_Signal* sig, void* data)
                {
                    can_frame* frame = (can_frame*)data;
                    double raw = dbcppp_SignalDecode(sig, frame->data);
                    double phys = dbcppp_SignalRawToPhys(sig, raw);
                    printf("\t%s=%f\n", dbcppp_SignalGetName(sig), phys);
                }
                dbcppp_MessageForEachSignal(msg, print_signal_data, &frame);
            }
        }
    }
}
```
# Decode-function
The signals decode function is using prestored masks and fixed offsets to speed up calculation, therefore the decoding-function should be almost as fast as a code generated decode function would be. The assembly of the `decode`-function on its critical path (signed and byte swap must happen) looks like this (VS19 10.0.18362.0 compiler):
```
template <Alignment aAlignment, Signal::ByteOrder aByteOrder, Signal::ValueType aValueType, Signal::ExtendedValueType aExtendedValueType>
double template_decode(const Signal* sig, const void* nbytes) noexcept
00007FF8025BCA73  mov         rax,rcx  
00007FF8025BCA76  mov         rcx,qword ptr [rcx+140h]  
00007FF8025BCA7D  xorps       xmm0,xmm0  
00007FF8025BCA80  bswap       r8  
00007FF8025BCA83  shr         r8,cl  
00007FF8025BCA86  and         r8,qword ptr [rax+130h]  
00007FF8025BCA8D  mov         rcx,qword ptr [rax+138h]  
00007FF8025BCA94  mov         rax,rcx  
00007FF8025BCA97  or          rcx,r8  
00007FF8025BCA9A  and         rax,r8  
00007FF8025BCA9D  cmove       rcx,r8  
00007FF8025BCAA1  cvtsi2sd    xmm0,rcx  
00007FF8025BCAA6  ret   
```
On the best path (no byteswap must take place and ExtendedValueType == Double) the decode function only has 5 instructions:
```
template <Alignment aAlignment, Signal::ByteOrder aByteOrder, Signal::ValueType aValueType, Signal::ExtendedValueType aExtendedValueType>
double template_decode(const Signal* sig, const void* nbytes) noexcept
00007FF8025BCAF0  mov         rax,qword ptr [rdx]  
00007FF8025BCAF3  mov         qword ptr [rsp+8],rcx  
00007FF8025BCAF8  mov         qword ptr [sig],rax  
00007FF8025BCAFD  movsd       xmm0,mmword ptr [data]  
00007FF8025BCB03  ret  
```
# Known issues
* tests for decoding function for float/double is failing on some maschines (currently only confirmed for System/s390x)
# Similar projects
  * [Vector_DBC](https://bitbucket.org/tobylorenz/vector_dbc/src/master/) Does basically the same, the biggest difference is that it uses `bison` instead of `boost::spirit` for grammar parsing
  * [CAN BUS tools in Python 3 (cantools)](https://github.com/eerimoq/cantools) 

