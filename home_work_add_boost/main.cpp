//
// Created by flyon21 on 06.01.25.
//
#include <iostream>
#include <boost/program_options.hpp>
#include <thread>
#include <sys/types.h>
#include <sys/utsname.h>
#include <cstdlib>
#include <unordered_map>


enum class Command {
    CPU,
    MEMORY,
    OS,
    ALL,
    UNKNOWN
};

void print_help(const boost::program_options::options_description& desc) {
    std::cout << "System Info Utility\n";
    std::cout << desc << "\n";
}

void print_cpu_info() {
    unsigned int cores = std::thread::hardware_concurrency();
    std::cout << "CPU Cores: " << cores << "\n";
}

void print_memory_info() {
#ifdef __linux__
    std::system("free -h");
#else
    std::cout << "Memory info is not supported on this OS.\n";
#endif
}

void print_os_info() {
    utsname buffer;
    if (uname(&buffer) == 0) {
        std::cout << "Operating System: " << buffer.sysname << "\n";
        std::cout << "Node Name: " << buffer.nodename << "\n";
        std::cout << "Release: " << buffer.release << "\n";
        std::cout << "Version: " << buffer.version << "\n";
        std::cout << "Machine: " << buffer.machine << "\n";
    } else {
        std::cerr << "Unable to retrieve OS info.\n";
    }
}

Command get_command(const std::string& command_str) {
    static const std::unordered_map<std::string, Command> command_map = {
            {"cpu", Command::CPU},
            {"memory", Command::MEMORY},
            {"os", Command::OS},
            {"all", Command::ALL}
    };

    auto it = command_map.find(command_str);
    if (it != command_map.end()) {
        return it->second;
    }
    return Command::UNKNOWN;
}

constexpr auto COMMAND_DESCRIPTIONS = R"(
System Info Utility
Usage:
  --help, -h     Print help message
  --cpu          Display CPU information
  --memory       Display memory information
  --os           Display OS information
  --all          Display all available system information
)";

void execute_command(Command command) {
    switch (command) {
        case Command::CPU:
            print_cpu_info();
        break;
        case Command::MEMORY:
            print_memory_info();
        break;
        case Command::OS:
            print_os_info();
        break;
        case Command::ALL:
            print_cpu_info();
        print_memory_info();
        print_os_info();
        break;
        default:
            std::cerr << "Error: Unknown command.\n"; // Handle unknown cases here.
        break;
    }
}


int main(int argc, char* argv[]) {
    try {
        boost::program_options::options_description options_desc("Allowed options");
        options_desc.add_options()
            ("help,h", "Print help message")
            ("cpu", "Display CPU information")
            ("memory", "Display memory information")
            ("os", "Display OS information")
            ("all", "Display all available system information");

        boost::program_options::variables_map options_map;
        boost::program_options::store(
            boost::program_options::parse_command_line(argc, argv, options_desc), options_map);
        boost::program_options::notify(options_map);

        if (options_map.count("help")) {
            print_help(options_desc);
            return 0;
        }

        if (options_map.empty()) {
            std::cout << "No options provided. Use --help to see available options.\n";
            return 0;
        }

        for (const auto& option : options_map) {
            Command command = get_command(option.first);
            execute_command(command);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}