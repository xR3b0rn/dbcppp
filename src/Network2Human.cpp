
#include <boost/format.hpp>
#include "dbcppp/Network2Functions.h"

using namespace dbcppp;

bool bit_is_inbetween(const ISignal& sig, std::size_t i_bit, uint64_t switch_value = -1)
{
    if (switch_value != -1 &&
        sig.MultiplexerIndicator() == ISignal::EMultiplexer::MuxValue &&
        sig.MultiplexerSwitchValue() != switch_value)
    {
        return false;
    }
    switch (sig.ByteOrder())
    {
    case ISignal::EByteOrder::LittleEndian:
    {
        std::size_t start = sig.StartBit();
        std::size_t end = sig.StartBit() + sig.BitSize();
        return start <= i_bit && end > i_bit;
    }
    case ISignal::EByteOrder::BigEndian:
    {
        std::vector<size_t> match_bits;
        std::size_t start = sig.StartBit();
        std::size_t n = sig.BitSize();
        while (n-- > 0) {
            match_bits.push_back(start);
            if (start > 15 && (start + 1) % 8 == 0)
            {
                start -= 15;
            }
            else
            {
                start++;
            }
        }
        if (std::find(match_bits.begin(), match_bits.end(), i_bit) != match_bits.end()) {
            return true;
        }
        else {
            return false;
        }
    }
    }
    return false;
}
bool is_start_bit(const ISignal& sig, std::size_t i_bit)
{
    return sig.StartBit() == i_bit;
}
bool is_end_bit(const ISignal& sig, std::size_t i_bit)
{
    switch (sig.ByteOrder())
    {
    case ISignal::EByteOrder::LittleEndian:
        return sig.StartBit() + sig.BitSize() - 1 == i_bit;
    case ISignal::EByteOrder::BigEndian:
    {
        std::size_t start = sig.StartBit();
        std::size_t n = sig.BitSize();
        std::size_t end = start;
        if (n > 0) {
            while (--n > 0) {
                if (end > 15 && (end + 1) % 8 == 0)
                {
                    end -= 15;
                }
                else
                {
                    end++;
                }
            }
            return end == i_bit;
        }
        else {
            return false;
        }
    }
    }
    return false;
}
const ISignal* get_mux_signal(const IMessage& msg)
{
    auto beg = msg.Signals().begin();
    auto end = msg.Signals().end();
    auto iter = std::find_if(beg, end,
        [&](const ISignal& sig)
        {
            return sig.MultiplexerIndicator() == ISignal::EMultiplexer::MuxSwitch;
        });
    return iter != end ? &*iter : nullptr;
}
std::set<uint64_t> get_mux_values(const IMessage& msg)
{
    std::set<uint64_t> mux_values;
    for (const ISignal& sig : msg.Signals())
    {
        if (sig.MultiplexerIndicator() == ISignal::EMultiplexer::MuxValue)
        {
            mux_values.insert(sig.MultiplexerSwitchValue());
        }
    }
    return mux_values;
}

DBCPPP_API std::ostream& dbcppp::Network2Human::operator<<(std::ostream& os, const IMessage& msg)
{
    os << boost::format("  %-12s%s\n") % "Name:" % msg.Name();
    os << boost::format("  %-12s0x%X\n") % "ID:" % msg.Id();
    os << boost::format("  %-12s0x%X\n") % "Length:" % msg.MessageSize();
    os << boost::format("  %-12s%s\n") % "Sender:" % msg.Transmitter();
    os << boost::format("  Layout:\n");
    
    auto mux_values = get_mux_values(msg);
    const ISignal* mux_sig = get_mux_signal(msg);
    
    std::vector<const ISignal*> sigs;
    for (const ISignal& sig : msg.Signals())
    {
        sigs.push_back(&sig);
    }
    std::size_t num_lines = 3 + msg.MessageSize() * 2 + sigs.size();
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
                    auto beg = msg.Signals().begin();
                    auto end = msg.Signals().end();
                    auto iter = std::find_if(beg, end, [&](const ISignal& sig) { return bit_is_inbetween(sig, i_bit, mux_value); });
                    return iter != end ? &*iter : nullptr;
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
                            while (i_bit > sig->StartBit() && i < 8)
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
                                os << boost::format(" +-- %s") % sig->Name();
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
            os << boost::format("               %s: %d\n") % mux_sig->Name()  % mux_value;
        };

    if (mux_values.size() == 0)
    {
        print_signal_header();
        i_bit = 7;
        while (i_bit < msg.MessageSize() * 8)
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
            while (i_bit < msg.MessageSize() * 8)
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
                os << boost::format("       +-- %s\n") % mux_sig->Name();
                std::size_t i_mux = mux_values.size();
                for (uint64_t mux_value : mux_values)
                {
                    os << boost::format("       |   +--%d\n") % mux_value;
                    for (const ISignal& sig : msg.Signals())
                    {
                        if (sig.MultiplexerIndicator() == ISignal::EMultiplexer::MuxValue &&
                            sig.MultiplexerSwitchValue() == mux_value)
                        {
                            if (i_mux > 1)
                            {
                                os << boost::format("       |   |  +-- %s\n") % sig.Name();
                            }
                            else
                            {
                                os << boost::format("       |      +-- %s\n") % sig.Name();
                            }
                        }
                    }
                    i_mux--;
                }
            }
            for (const ISignal& sig : msg.Signals())
            {
                if (sig.MultiplexerIndicator() == ISignal::EMultiplexer::NoMux)
                {
                    os << boost::format("       +-- %s\n") % sig.Name();
                }
            }
        };
    print_signal_tree();
    os << boost::format("\n");
    auto print_value_tables =
        [&]()
        {
            os << boost::format("  Signal choices:\n\n");
            for (const ISignal& sig : msg.Signals())
            {
                if (sig.ValueEncodingDescriptions_Size())
                {
                    os << boost::format("    %s\n") % sig.Name();
                    for (const IValueEncodingDescription& ved : sig.ValueEncodingDescriptions())
                    {
                        os << boost::format("        %d %s\n") % ved.Value() % ved.Description();
                    }
                    os << boost::format("\n");
                }
            }
        };
    print_value_tables();
    return os;
}
DBCPPP_API std::ostream& dbcppp::Network2Human::operator<<(std::ostream& os, const INetwork& net)
{
    os << boost::format("================================= Messages =================================\n");
    for (const IMessage& msg : net.Messages())
    {
        os << msg;
        os << boost::format("  ------------------------------------------------------------------------\n");
    }
    return os;
}
