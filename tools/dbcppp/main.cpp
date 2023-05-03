#include <regex>
#include <array>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <memory>

#include <cxxopts.hpp>

#include "dbcppp/Network.h"
#include "dbcppp/Network2Functions.h"

void print_help()
{
    std::cout << "dbcppp v1.0.0\nFor help type: dbcppp <subprogram> --help\n"
        << "Sub programs: dbc2, decode\n";
}

int main(int argc, char** argv)
{
    cxxopts::Options options("dbcppp", "");
    if (argc < 2 || std::string("help") == argv[1])
    {
        print_help();
        return 1;
    }

    if (std::string("dbc2") == argv[1])
    {
        options.add_options()
            ("h,help", "Produce help message")
            ("f,format", "Output format (C, DBC, human)", cxxopts::value<std::string>())
            ("dbc", "List of DBC files", cxxopts::value<std::vector<std::string>>());

        for (std::size_t i = 1; i < argc - 1; i++)
        {
            argv[i] = argv[i + 1];
        }
        auto vm = options.parse(argc - 1, argv);

        if (vm.count("help"))
        {
            std::cout << "Usage:\ndbcppp dbc2c [--help] --format=<format>... --dbc=<dbc filename>...\n";
            std::cout << options.help();
            return 1;
        }
        if (!vm.count("format"))
        {
            std::cout << "Argument error: Argument --format=<format> missing\n";
            return 1;
        }
        if (!vm.count("dbc"))
        {
            std::cout << "Argument error: At least one --dbc=<dbc> argument required\n";
            return 1;
        }
        const auto& format = vm["format"].as<std::string>();
        auto dbcs = vm["dbc"].as<std::vector<std::string>>();
        auto net = dbcppp::INetwork::Create({}, {}, dbcppp::IBitTiming::Create(0, 0, 0), {}, {}, {}, {}, {}, {}, {}, {});
        for (const auto& dbc : dbcs)
        {
            auto nets = dbcppp::INetwork::LoadNetworkFromFile(dbc);
            for (auto& other : nets)
            {
                net->Merge(std::move(other.second));
            }
        }
        if (format == "C")
        {
            using namespace dbcppp::Network2C;
            std::cout << *net;
        }
        else if (format == "DBC")
        {
            using namespace dbcppp::Network2DBC;
            std::cout << *net;
        }
        else if (format == "human")
        {
            using namespace dbcppp::Network2Human;
            std::cout << *net;
        }
    }
    else if (std::string("decode") == argv[1])
    {
        options.add_options()
            ("h,help", "Produce help message")
            ("bus", "List of buses in format <<bus name>:<DBC filename>>", cxxopts::value<std::vector<std::string>>());
        for (std::size_t i = 1; i < argc - 1; i++)
        {
            argv[i] = argv[i + 1];
        }
        auto vm = options.parse(argc, argv);
        if (vm.count("help"))
        {
            std::cout << "Usage:\ndbcppp decode [--help] --bus=<<bus name>:<DBC filename>>...\n";
            std::cout << options.help();
            return 1;
        }
        if (!vm.count("bus"))
        {
            std::cout << "Argument error: At least one --bus=<<bus name>:<DBC filename>> argument required\n";
            return 1;
        }
        const auto& opt_buses = vm["bus"].as<std::vector<std::string>>();
        struct Bus
        {
            std::string name;
            std::unique_ptr<dbcppp::INetwork> net;
        };
        std::unordered_map<std::string, Bus> buses;
        for (const auto& opt_bus : opt_buses)
        {
            std::istringstream ss(opt_bus);
            std::string opt;
            Bus b;
            if (std::getline(ss, opt, ':'))
            {
                b.name = opt;
            }
            else
            {
                std::cout << "error: could parse bus parameter" << std::endl;
                return 1;
            }
            if (std::getline(ss, opt))
            {
                std::ifstream fdbc(opt);
                b.net = dbcppp::INetwork::LoadDBCFromIs(fdbc);
                if (!b.net)
                {
                    std::cout << "error: could not load DBC '" << opt << "'" << std::endl;
                    return 1;
                }
            }
            else
            {
                std::cout << "error: could parse bus parameter" << std::endl;
                return 1;
            }
            buses.insert(std::make_pair(b.name, std::move(b)));
        }
        // example line: vcan0  123   [3]  11 22 33
        std::regex regex_candump_line(
            // vcan0
            "^\\s*(\\S+)"
            // 123
            "\\s*([0-9A-F]{3})"
            // 3
            "\\s*\\[(\\d+)\\]"
            // 11
            "\\s*([0-9A-F]{2})?"
            // 22
            "\\s*([0-9A-F]{2})?"
            // 33
            "\\s*([0-9A-F]{2})?"
            // ...
            "\\s*([0-9A-F]{2})?"
            "\\s*([0-9A-F]{2})?"
            "\\s*([0-9A-F]{2})?"
            "\\s*([0-9A-F]{2})?"
            "\\s*([0-9A-F]{2})?");
        std::string line;
        while (std::getline(std::cin, line))
        {
            std::cmatch cm;
            std::regex_match(line.c_str(), cm, regex_candump_line);
            const auto& bus = buses.find(cm[1].str());
            if (bus != buses.end())
            {
                uint64_t msg_id = std::strtol(cm[2].str().c_str(), nullptr, 16);
                uint64_t msg_size = std::atoi(cm[3].str().c_str());
                std::array<uint8_t, 8> data;
                for (std::size_t i = 0; i < msg_size; i++)
                {
                    data[i] = uint8_t(std::strtol(cm[4 + i].str().c_str(), nullptr, 16));
                }
                auto beg_msg = bus->second.net->Messages().begin();
                auto end_msg = bus->second.net->Messages().end();
                auto iter = std::find_if(beg_msg, end_msg, [&](const dbcppp::IMessage& msg) { return msg.Id() == msg_id; });
                if (iter != end_msg)
                {
                    const dbcppp::IMessage* msg = &*iter;
                    std::cout << line << " :: " << msg->Name() << "(";
                    bool first = true;
                    const auto* mux_sig = msg->MuxSignal();

                    auto print_signal =
                        [&data](const dbcppp::ISignal& sig, bool first)
                        {
                            if (!first) std::cout << ", ";
                            auto raw = sig.Decode(&data[0]);
                            auto beg_ved = sig.ValueEncodingDescriptions().begin();
                            auto end_ved = sig.ValueEncodingDescriptions().end();
                            auto iter = std::find_if(beg_ved, end_ved, [&](const dbcppp::IValueEncodingDescription& ved) { return ved.Value() == raw; });
                            if (iter != end_ved)
                            {
                                std::cout << sig.Name() << ": '" << iter->Description() << "' " << sig.Unit();
                            }
                            else
                            {
                                auto val = sig.RawToPhys(raw);
                                std::cout << sig.Name() << ": " << val;
                                if (sig.Unit().size())
                                {
                                    std::cout << " " << sig.Unit();
                                }
                            }
                        };

                    for (const dbcppp::ISignal& sig : msg->Signals())
                    {
                        if (sig.MultiplexerIndicator() != dbcppp::ISignal::EMultiplexer::MuxValue)
                        {
                            print_signal(sig, first);
                            first = false;
                        }
                        else if (mux_sig && sig.SignalMultiplexerValues_Size() == 0 &&
                            sig.MultiplexerSwitchValue() == mux_sig->Decode(&data[0]))
                        {
                            print_signal(sig, first);
                            first = false;
                        }
                        else
                        {
                            std::function<bool(const dbcppp::ISignal&)> check_signal_multiplexer_values;
                            check_signal_multiplexer_values =
                                [&](const dbcppp::ISignal& sig)
                                    -> bool
                                {
                                    for (const auto& smv : sig.SignalMultiplexerValues())
                                    {
                                        auto sig_beg = msg->Signals().begin();
                                        auto sig_end = msg->Signals().end();
                                        auto sig_iter = std::find_if(sig_beg, sig_end,
                                            [&](const auto& sig)
                                            {
                                                return sig.Name() == smv.SwitchName();
                                            });
                                        if (sig_iter != sig_end)
                                        {
                                            for (auto ranges : smv.ValueRanges())
                                            {
                                                auto raw = sig_iter->Decode(&data[0]);
                                                if (ranges.from >= raw && ranges.to <= raw)
                                                {
                                                    if (sig_iter->SignalMultiplexerValues_Size() != 0)
                                                    {
                                                        return check_signal_multiplexer_values(*sig_iter);
                                                    }
                                                    else
                                                    {
                                                        return true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    return false;
                                };
                            if (check_signal_multiplexer_values(sig))
                            {
                                print_signal(sig, first);
                                first = false;
                            }
                        }
                    }
                    std::cout << ")\n";
                }
            }
        }
    }
    else
    {
        print_help();
        return 1;
    }
}
