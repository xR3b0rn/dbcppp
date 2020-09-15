
#include <boost/format.hpp>
#include "../../include/dbcppp/Network2Functions.h"

using namespace dbcppp;

bool bit_is_inbetween(const Signal& sig, std::size_t i_bit, uint64_t switch_value = -1)
{
    if (switch_value != -1 &&
        sig.getMultiplexerIndicator() == Signal::Multiplexer::MuxValue &&
        sig.getMultiplexerSwitchValue() != switch_value)
    {
        return false;
    }
    switch (sig.getByteOrder())
    {
    case Signal::ByteOrder::LittleEndian:
    {
        std::size_t start = sig.getStartBit();
        std::size_t end = sig.getStartBit() + sig.getBitSize();
        return start <= i_bit && end > i_bit;
    }
    case Signal::ByteOrder::BigEndian:
    {
        std::size_t start = sig.getStartBit();
        std::size_t n = sig.getBitSize();
        while (n)
        {
            if (start == i_bit)
            {
                return true;
            }
            if (start % 8 == 0)
            {
                start += 15;
            }
            else
            {
                start--;
            }
            n--;
        }
        return true;
    }
    }
    return false;
}
bool is_start_bit(const Signal& sig, std::size_t i_bit)
{
    switch (sig.getByteOrder())
    {
    case Signal::ByteOrder::LittleEndian:
        return sig.getStartBit() == i_bit;
    case Signal::ByteOrder::BigEndian:
        break;
    }
    return false;
}
bool is_end_bit(const Signal& sig, std::size_t i_bit)
{
    switch (sig.getByteOrder())
    {
    case Signal::ByteOrder::LittleEndian:
        return sig.getStartBit() + sig.getBitSize() - 1 == i_bit;
    case Signal::ByteOrder::BigEndian:
        break;
    }
    return false;
}
const Signal* get_mux_signal(const Message& msg)
{
    return msg.findSignal(
        [](const Signal& sig)
        {
            return sig.getMultiplexerIndicator() == Signal::Multiplexer::MuxSwitch;
        });
}
std::set<uint64_t> get_mux_values(const Message& msg)
{
    std::set<uint64_t> result;
    msg.forEachSignal(
        [&](const Signal& sig)
        {
            if (sig.getMultiplexerIndicator() == Signal::Multiplexer::MuxValue)
            {
                result.insert(sig.getMultiplexerSwitchValue());
            }
        });
    return result;
}

DBCPPP_API std::ostream& dbcppp::Network2Human::operator<<(std::ostream& os, const Message& msg)
{
    os << boost::format("  %-12s%s\n") % "Name:" % msg.getName();
    os << boost::format("  %-12s0x%X\n") % "ID:" % msg.getId();
    os << boost::format("  %-12s0x%X\n") % "Length:" % msg.getMessageSize();
    os << boost::format("  %-12s%s\n") % "Sender:" % msg.getTransmitter();
    os << boost::format("  Layout:\n");
    
    auto mux_values = get_mux_values(msg);
    const Signal* mux_sig = get_mux_signal(msg);
    
    std::vector<const Signal*> sigs;
    msg.forEachSignal(
        [&](const Signal& sig)
        {
            sigs.push_back(&sig);
        });
    std::size_t num_lines = 3 + msg.getMessageSize() * 2 + sigs.size();
    std::size_t i = 0;
    std::size_t i_bit = 7;
    int64_t i_byte_str = int64_t(num_lines) / 2 + 2 - int64_t(num_lines);
    auto print_ident =
        [&]()
        {
            os << boost::format("         ");
        };
    auto print_bar =
        [&]()
        {
            print_ident();
            os << boost::format("  +---+---+---+---+---+---+---+---+\n");
        };
    auto print_signal_header =
        [&]()
        {
            print_ident();
            os << boost::format("                 Bit\n");
            print_ident();
            os << boost::format("    7   6   5   4   3   2   1   0\n");
        };
    bool print_last_line = false;
    auto print_signal =
        [&](uint64_t mux_value = -1)
        {
            print_bar();
            print_ident();
            os << boost::format("%d |") % (i_bit / 8);
            auto find_cur_sig =
                [&]()
                {
                    return msg.findSignal(
                        [&](const Signal& sig)
                        {
                            return bit_is_inbetween(sig, i_bit, mux_value);
                        });
                };
            int64_t depth = 0;
            for (std::size_t i = 0; i < 8; i++)
            {
                const auto* sig = find_cur_sig();
                if (sig)
                {
                    if (is_start_bit(*sig, i_bit) && is_end_bit(*sig, i_bit))
                    {
                        os << boost::format("<-x|");
                        depth++;
                    }
                    else if (is_start_bit(*sig, i_bit))
                    {
                        os << boost::format("--x|");
                        depth++;
                    }
                    else if (is_end_bit(*sig, i_bit))
                    {
                        os << boost::format("<--");
                        if (i_bit % 8 == 0)
                        {
                            os << boost::format("|");
                        }
                        else
                        {
                            os << boost::format("-");
                        }
                    }
                    else
                    {
                        os << boost::format("---");
                        if (i_bit % 8 == 0)
                        {
                            os << boost::format("|");
                        }
                        else
                        {
                            os << boost::format("-");
                        }
                    }
                }
                else
                {
                    os << boost::format("   |");
                }
                i_bit--;
            }
            os << boost::format("\n");
            if (depth)
            {
                print_last_line = false;
                print_ident();
                os << boost::format("  +---+---+---+---+---+---+---+---+\n");
            }
            else
            {
                print_last_line = true;
            }
            i_bit += 8;
            if (depth)
            {
                print_ident();
                os << boost::format("   ");
                std::size_t n_sigs = 1;
                for (; 0 < depth; depth--)
                {
                    n_sigs = 0;
                    for (std::size_t i = 0; i < 8; i++)
                    {
                        const auto* sig = find_cur_sig();
                        if (sig)
                        {
                            while (i_bit > sig->getStartBit() && i < 8)
                            {
                                os << boost::format("    ");
                                i_bit--;
                                i++;
                            }
                            if (i == 8)
                            {
                                continue;
                            }
                            if (depth - 1 == n_sigs)
                            {
                                os << boost::format(" +-- %s") % sig->getName();
                            }
                            else if (n_sigs < std::size_t(depth - 1))
                            {
                                os << boost::format(" |  ");
                            }
                            n_sigs++;
                        }
                        else
                        {
                            os << boost::format("    ");
                        }
                        i_bit--;
                    }
                    i_bit += 8;
                    os << boost::format("\n");
                    if (depth != 1)
                    {
                        print_ident();
                        os << boost::format("   ");
                    }
                }
            }
            i_bit += 8;
        };
    auto print_mux_header =
        [&](uint64_t mux_value)
        {
            print_ident();
            os << boost::format("               %s: %d\n") % mux_sig->getName()  % mux_value;
        };

    if (mux_values.size() == 0)
    {
        print_signal_header();
        i_bit = 7;
        while (i_bit < msg.getMessageSize() * 8)
        {
            print_signal();
        }
        if (print_last_line)
        {
            print_ident();
            os << boost::format("  +---+---+---+---+---+---+---+---+\n");
        }
    }
    else
    {
        for (uint64_t mux_value : mux_values)
        {
            print_mux_header(mux_value);
            print_signal_header();
            i_bit = 7;
            while (i_bit < msg.getMessageSize() * 8)
            {
                print_signal(mux_value);
            }
            if (print_last_line)
            {
                print_ident();
                os << boost::format("  +---+---+---+---+---+---+---+---+\n");
            }
        }
    }
    os << boost::format("\n");
    auto print_signal_tree =
        [&]()
        {
            os << boost::format("  Signal tree:\n\n");
            os << boost::format("    -- {root}\n");
            if (mux_sig)
            {
                os << boost::format("       +-- %s\n") % mux_sig->getName();
                std::size_t i_mux = mux_values.size();
                for (uint64_t mux_value : mux_values)
                {
                    os << boost::format("       |   +--%d\n") % mux_value;
                    msg.forEachSignal(
                        [&](const Signal& sig)
                        {
                            if (sig.getMultiplexerIndicator() == Signal::Multiplexer::MuxValue &&
                                sig.getMultiplexerSwitchValue() == mux_value)
                            {
                                if (i_mux > 1)
                                {
                                    os << boost::format("       |   |  +-- %s\n") % sig.getName();
                                }
                                else
                                {
                                    os << boost::format("       |      +-- %s\n") % sig.getName();
                                }
                            }
                        });
                    i_mux--;
                }
            }
            msg.forEachSignal(
                [&](const Signal& sig)
                {
                    if (sig.getMultiplexerIndicator() == Signal::Multiplexer::NoMux)
                    {
                        os << boost::format("       +-- %s\n") % sig.getName();
                    }
                });
        };
    print_signal_tree();
    os << boost::format("\n");
    auto print_value_tables =
        [&]()
        {
            os << boost::format("  Signal choices:\n\n");
            msg.forEachSignal(
                [&](const Signal& sig)
                {
                    bool first = true;
                    sig.forEachValueDescription(
                        [&](int64_t value, const std::string& name)
                        {
                            if (first)
                            {
                                os << boost::format("    %s\n") % sig.getName();
                                first = false;
                            }
                            os << boost::format("        %d %s\n") % value % name;
                        });
                    if (!first)
                    {
                        os << boost::format("\n");
                    }
                });
        };
    print_value_tables();
    return os;
}
DBCPPP_API std::ostream& dbcppp::Network2Human::operator<<(std::ostream& os, const Network& net)
{
    os << boost::format("================================= Messages =================================\n");
    net.forEachMessage(
        [&](const Message& msg)
        {
            os << msg;
            os << boost::format("  ------------------------------------------------------------------------\n");
        });
    return os;
}
