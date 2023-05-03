
#include <fstream>
#include <unordered_map>

#include "dbcppp/CApi.h"
#include "dbcppp/Network.h"

// from uapi/linux/can.h
using canid_t = uint32_t;
struct can_frame
{
	canid_t    can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
	uint8_t    can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
	uint8_t    __pad;   /* padding */
	uint8_t    __res0;  /* reserved / padding */
	uint8_t    __res1;  /* reserved / padding */
	uint8_t    data[8];
};

void receive_frame_data(can_frame* frame)
{
    // receive meaningful data


    // Example frame
    // Set data for test_msg_mux0 from src/Test/Test.dbc
    // The selcted mux does have the format
    /* BO_ 1 test_msg_mux0: 8 VehicleBus
     *  [...]
     *  SG_ s3_0 m3 : 8|8@1+ (1,-40) [0|0] "C" Receiver
     *  SG_ s3_1 m3 : 16|11@1+ (0.5,1) [0|0] "C" Receiver
     *  SG_ s3_2 m3 : 27|10@1+ (1,-20) [0|0] "C" Receiver
     *  SG_ s3_3 m3 : 37|10@1+ (1,0) [0|0] "C" Receiver
     *  SG_ s3_4 m3 : 47|1@1+ (1,0) [0|0] "" Receiver
     *  SG_ s3_5 m3 : 48|1@1+ (1,0) [0|0] "" Receiver
     *  SG_ s3_6 m3 : 49|1@1+ (1,0) [0|0] "" Receiver
     *  SG_ s3_7 m3 : 50|11@1+ (0.125,-40) [0|0] "C" Receiver
     *  SG_ s3_8 m3 : 61|1@1+ (1,0) [0|0] "" Receiver
     *  SG_ s3_9 m3 : 62|1@1+ (1,0) [0|0] "" Receiver
     *  SG_ s3_10 m3 : 63|1@1+ (1,0) [0|0] "" Receiver
     *  [...]
     */
    frame->can_id = 1;
    *reinterpret_cast<uint64_t*>(frame->data) = 0;
    // set mux_switch_value to 3 (m3)
    frame->data[0] |= 3;
    // set value for signal s3_1 to 13
    // expected output after decoding and rawToPhys: s3_1 = 15 * 0.5 + 1 = 8.5
    frame->data[2] |= 15;
}
int main()
{
    std::unique_ptr<dbcppp::INetwork> net;
    {
        std::ifstream idbc("your.dbc");
        net = dbcppp::INetwork::LoadDBCFromIs(idbc);
    }

    if (net.get() == nullptr) {
        std::cerr << "failed to parse!\n";
        return -1;
    }

    std::unordered_map<uint64_t, const dbcppp::IMessage *> messages;
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
    std::cout << std::flush;
}
