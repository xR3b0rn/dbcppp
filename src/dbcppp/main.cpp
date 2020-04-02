
#include <array>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>

#include <robin-map/tsl/robin_map.h>
#include "../../include/dbcppp/Network.h"
#include "../../include/dbcppp/Network2C.h"
#include "../../include/dbcppp/Network2DBC.h"

int main(int argc, char** args)
{
    namespace po = boost::program_options;
    if (argc < 2)
    {
        // print help
        return 1;
    }
    if (std::string("help") == args[1])
    {
        std::cout << "For more help type:\ndbcppp <subprogram> --help\n"
            << "Sub programs are: dbc2, decode\n";
    }
    po::positional_options_description p;
    p.add("subprogram", -1);
    if (std::string("dbc2") == args[1])
    {
        po::options_description desc("Options");
        desc.add_options()
            ("help", "produce help message")
            ("format,f", po::value<std::string>()->required(), "output format (C, DBC)")
            ("dbc", po::value<std::vector<std::string>>()->multitoken()->required(), "list of DBC files")
            ("out,o", po::value<std::string>()->default_value("a.out"), "output filename")
            ("subprogram", po::value<std::string>()->required(), "sub program");

        po::variables_map vm;
        po::store(po::command_line_parser(argc, args).options(desc).positional(p).run(), vm);
        po::notify(vm);    

        if (vm.count("help"))
        {
            std::cout << "Usage:\ndbcppp dbc2c --format=<format>... --dbc=<dbc filename>... [--out=<output filename>]\n";
            std::cout << desc << "\n";
            return 1;
        }
        const auto& format = vm["format"].as<std::string>();
        const auto& out = vm["out"].as<std::string>();
        auto dbcs = vm["dbc"].as<std::vector<std::string>>();
        std::ifstream fdbc(dbcs[0]);
        dbcs.erase(dbcs.begin());
        auto net = dbcppp::Network::fromDBC(fdbc);
        for (const auto& dbc : dbcs)
        {
            fdbc = std::ifstream(dbc);
            auto other = dbcppp::Network::fromDBC(fdbc);
            net->merge(std::move(other));
        }
        std::ofstream fout(out);
        if (format == "C")
        {
            using namespace dbcppp::Network2C;
            fout << *net;
        }
        else if (format == "DBC")
        {
            using namespace dbcppp::Network2DBC;
            fout << *net;
        }
    }
    else if (std::string("decode") == args[1])
    {
        po::options_description desc("Options");
        desc.add_options()
            ("help", "produce help message")
            ("bus", po::value<std::vector<std::string>>()->required(), "list of DBC files in format (<bus name, DBC filename>)")
            ("subprogram", po::value<std::string>()->required(), "sub program");

        po::variables_map vm;
        po::store(po::command_line_parser(argc, args).options(desc).positional(p).run(), vm);
        po::notify(vm);    

        if (vm.count("help"))
        {
            std::cout << "Usage:\ndbcppp decode --dbc=<bus name,DBC filename>...\n";
            std::cout << desc << "\n";
            return 1;
        }
        const auto& opt_buses = vm["bus"].as<std::vector<std::string>>();
        struct Bus
        {
            std::string name;
            std::unique_ptr<dbcppp::Network> net;
        };
        tsl::robin_map<std::string, Bus> buses;
        for (const auto& opt_bus : opt_buses)
        {
            std::istringstream ss(opt_bus);
            std::string opt;
            Bus b;
            if (std::getline(ss, opt, ','))
            {
                b.name = opt;
            }
            else
            {
                // TODO error
            }
            if (std::getline(ss, opt))
            {
                std::ifstream fdbc(opt);
                b.net = dbcppp::Network::fromDBC(fdbc);
            }
            else
            {
                // TODO error
            }
            buses.insert(std::make_pair(b.name, std::move(b)));
        }
        std::string line;
        while (std::getline(std::cin, line))
        {
            //vcan0  123   [8]  11 22 33 44 55 66 77 88
            const char* a = line.c_str();
            const char* b = a;
            while (*b != ' ') b++;
            const auto& bus = buses.find(std::string(a, std::distance(a, b)));
            if (bus != buses.end())
            {
                while (*b == ' ') b++;
                a = b;
                while (*b != ' ') b++;
                uint64_t msg_id = std::strtol(std::string(a, std::distance(a, b)).c_str(), nullptr, 16);
                while (*b == ' ') b++;
                a = b + 1;
                b += 3;
                while (*b == ' ') b++;
                uint64_t msg_size = std::atoi(std::string(a, 1).c_str());
                a = b;
                std::array<uint8_t, 8> data;
                for (std::size_t i = 0; i < msg_size; i++)
                {
                    data[i] = uint8_t(std::strtol(std::string(a + i * 3, 2).c_str(), nullptr, 16));
                }
                const dbcppp::Message* msg = bus->second.net->getMessageById(msg_id);
                if (msg)
                {
                    std::cout << line << " :: " << msg->getName() << "(";
                    bool first = true;
                    const auto* mux_sig = msg->getMuxSignal();
                    msg->forEachSignal(
                        [&](const dbcppp::Signal& sig)
                        {
                            if (sig.getMultiplexerIndicator() == dbcppp::Signal::Multiplexer::MuxValue &&
                                mux_sig && sig.getMultiplexerSwitchValue() == mux_sig->decode(&data[0]))
                            {
                                if (first) first = false; else std::cout << ", ";
                                auto raw = sig.decode(&data[0]);
                                auto desc = sig.getValueDescriptionByValue(raw);
                                if (desc != nullptr)
                                {
                                    std::cout << sig.getName() << ": " << desc << " " << sig.getUnit();
                                }
                                else
                                {
                                    auto val = sig.rawToPhys(raw);
                                    std::cout << sig.getName() << ": " << val << " " << sig.getUnit();
                                }
                            }
                        });
                    std::cout << ")\n";
                }
            }
        }
    }
}
