
#include <fstream>

#include "../../include/dbcppp/CApi.h"
#include "../../include/dbcppp/Network.h"
#include "../../Test/Config.h"
#define DBCPPP_BYTE_ORDER_LITTLE_ENDIAN 1
#include "test.h"

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
    std::unique_ptr<dbcppp::Network> net;
    {
        std::ifstream idbc(TEST_DBC);
        net = dbcppp::Network::fromDBC(idbc);
    }
    can_frame frame;
    receive_frame_data(&frame);
    const dbcppp::Message* msg = net->getMessageById(frame.can_id);
    std::cout << "Received Message: " << msg->getName() << "\n";
    msg->forEachSignal(
        [&](const dbcppp::Signal& sig)
        {
            const dbcppp::Signal* mux_sig = msg->getMuxSignal();
            if (sig.getMultiplexerIndicator() != dbcppp::Signal::Multiplexer::MuxValue ||
                (mux_sig && mux_sig->decodeDiscrete(frame.data) == sig.getMultiplexerSwitchValue()))
            {
                std::cout << "\t" << sig.getName() << "=" << sig.rawToPhys(sig.decode(frame.data)) << sig.getUnit() << "\n";
            }
        });
    std::cout << std::flush;
}
