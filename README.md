[![Build Status](https://github.com/xR3b0rn/dbcppp/actions/workflows/linux.yml/badge.svg?branch=master)
# dbcppp
A C/C++ DBC file parser based on `boost.spirit`. This library is designed for decoding performance.
# Features
* very fast decoding
* verbose parser output in error case
* DBC is editable through C/C++ interface exported from the library
* read/write DBC file
* decode functionality for frames of arbitrarily byte length
* [cantools](https://github.com/eerimoq/cantools) like decoding
* [KCD](https://github.com/julietkilo/kcd) file format support


# Getting started
## Build & Install
```
git clone --recurse-submodules https://github.com/xR3b0rn/dbcppp.git
cd dbcppp
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j
make RunTests
make install
ldconfig # on Unix-systems only
```

# Usage example
## Command line tool
### dbc2
```
# generate C source from DBC/KCD
dbcparser dbc2 --dbc=file.dbc --format=C
# beauty or merge DBC/KCD
dbcparser dbc2 --dbc=file1.dbc --dbc=file2.kcd --format=DBC
# print DBC/KCD in human readable format
dbcparser dbc2 --dbc=file1.dbc --dbc=file2.kcd --format=human
```
### decode
[cantools](https://github.com/eerimoq/cantools) like decoding:
```
candump any | dbcppp decode --bus=vcan0,file1.dbc --bus=vcan1,file2.dbc
```
## Library
* [Examples](https://github.com/xR3b0rn/dbcppp/tree/master/examples)
* `C++`
```C++
#include <fstream>
#include <dbcppp/Network.h>
int main()
{
    std::unique_ptr<dbcppp::INetwork> net;
    {
        std::ifstream idbc("your.dbc");
        net = dbcppp::INetwork::LoadDBCFromIs(idbc);
    }
    std::unordered_map<uint64_t, const dbcppp::IMessage*> messages;
    for (const dbcppp::IMessage& msg : net->Messages())
    {
        messages.insert(std::make_pair(msg.Id(), &msg));
    }
    can_frame frame;
    while (1)
    {
        receive_frame_data(&frame);
        auto iter = messages.find(frame.can_id);
        if (iter != messages.end())
        {
            const dbcppp::IMessage* msg = iter->second;
            std::cout << "Received Message: " << msg->Name() << "\n";
            for (const dbcppp::ISignal& sig : msg->Signals())
            {
                const dbcppp::ISignal* mux_sig = msg->MuxSignal();
                if (sig.MultiplexerIndicator() != dbcppp::ISignal::EMultiplexer::MuxValue ||
                    (mux_sig && mux_sig->Decode(frame.data) == sig.MultiplexerSwitchValue()))
                {
                    std::cout << "\t" << sig.Name() << "=" << sig.RawToPhys(sig.Decode(frame.data)) << sig.Unit() << "\n";
                }
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
    const dbcppp_Nework* net = dbcppp_NetworkLoadDBCFromFile("your.dbc");
    if (net)
    {
        can_frame frame;
        while (1)
        {
            receive_can_frame_from_somewhere(&frame);
            const dbcppp_Message* msg = nullptr;
            auto n_msgs = dbcppp_NetworkMessages_Size(net);
            for (uint64_t i = 0; i < n_msgs; i++)
            {
                const dbcppp_Message* msg = dbcppp_NetworkMessages_Get(i);
                if (dbcppp_MessageId(tmp) == frame.can_id)
                {
                    printf("Received message: %s\n", dbcppp_MessageGetName(msg));
                    dbcppp_MessageForEachSignal(msg, print_signal_data, &frame);
                    for (uint64_t i = 0; i < dbcppp_MessageSignals_Size(msg); i++)
                    {
                        const dbcppp_Signal* sig = dbcppp_MessageSignals_Get(msg, i);
                        uint64_t raw = dbcppp_SignalDecode(sig, frame.data);
                        double phys = dbcppp_SignalRawToPhys(sig, raw);
                        printf("\t%s=%f\n", dbcppp_SignalName(sig), phys);
                    }
                }
            }
        }
    }
    dbcppp_NetworkFree(net);
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
* signal_groups
* signal_multiplexer_value
## Not supported yet
* sigtype_attr_list
* signal_type_refs
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
* tests for decoding function for float/double is failing on some machines (currently only confirmed for System/s390x)
# Similar projects
  * [Vector_DBC](https://bitbucket.org/tobylorenz/vector_dbc/src/master/) Does basically the same, the biggest difference is that it uses `bison` instead of `boost::spirit` for grammar parsing
  * [CAN BUS tools in Python 3 (cantools)](https://github.com/eerimoq/cantools) 
