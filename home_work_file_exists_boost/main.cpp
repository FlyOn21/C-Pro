//
// Created by flyon21 on 14.01.25.
//

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <unordered_map>
#include <thread>
#include <stdexcept>

namespace fs = boost::filesystem;
namespace po = boost::program_options;

enum class SizeValue {
    B,
    KB,
    MB,
    GB
};

enum class Command {
    SIZE,
    LAST_MODIFIED,
    PERMISSIONS,
    ALL,
    UNKNOWN
};

constexpr auto COMMAND_DESCRIPTIONS = R"(
File Info Utility
Usage:
  --help, -h          Print help message
  --files, -f <path>  File path, required
  --size <value>      File size. Possible values: B, KB, MB, GB, default is B
  --last-modified     Last modified date
  --permissions       File permissions
  --all              Execute all commands
)";

boost::optional<SizeValue> get_size_value(const std::string& size) {
    static const std::unordered_map<std::string, SizeValue> size_map = {
        {"B", SizeValue::B},
        {"KB", SizeValue::KB},
        {"MB", SizeValue::MB},
        {"GB", SizeValue::GB}
    };
    auto it = size_map.find(size);
    if (it != size_map.end()) {
        return boost::optional<SizeValue>(it->second);
    }
    return boost::none;
}

void print_help(const po::options_description& desc) {
    std::cout << "File Info Utility\n";
    std::cout << desc << "\n";
    std::cout << COMMAND_DESCRIPTIONS << "\n";
}

std::string human_readable_permissions(fs::perms p) {
    std::string permStr;

    // Owner permissions
    permStr += ((p & fs::perms::owner_read) != fs::perms::no_perms) ? 'r' : '-';
    permStr += ((p & fs::perms::owner_write) != fs::perms::no_perms) ? 'w' : '-';
    permStr += ((p & fs::perms::owner_exe) != fs::perms::no_perms) ? 'x' : '-';

    permStr += ' ';

    // Group permissions
    permStr += ((p & fs::perms::group_read) != fs::perms::no_perms) ? 'r' : '-';
    permStr += ((p & fs::perms::group_write) != fs::perms::no_perms) ? 'w' : '-';
    permStr += ((p & fs::perms::group_exe) != fs::perms::no_perms) ? 'x' : '-';

    permStr += ' ';

    // Others permissions
    permStr += ((p & fs::perms::others_read) != fs::perms::no_perms) ? 'r' : '-';
    permStr += ((p & fs::perms::others_write) != fs::perms::no_perms) ? 'w' : '-';
    permStr += ((p & fs::perms::others_exe) != fs::perms::no_perms) ? 'x' : '-';

    return permStr;
}

void execute_command(Command command, const fs::path& file, const boost::optional<SizeValue>& size_value = boost::none) {
    switch (command) {
        case Command::SIZE: {
            auto file_size = fs::file_size(file);
            double converted_size = static_cast<double>(file_size);
            SizeValue actual_size_value = size_value.get_value_or(SizeValue::B);

            if (actual_size_value == SizeValue::KB) converted_size /= 1024.0;
            if (actual_size_value == SizeValue::MB) converted_size /= 1024.0 * 1024.0;
            if (actual_size_value == SizeValue::GB) converted_size /= 1024.0 * 1024.0 * 1024.0;

            std::cout << "File size: " << converted_size << " "
                      << (actual_size_value == SizeValue::B ? "B" :
                          actual_size_value == SizeValue::KB ? "KB" :
                          actual_size_value == SizeValue::MB ? "MB" : "GB") << "\n";
            break;
        }
        case Command::LAST_MODIFIED: {
            auto last_write_time = fs::last_write_time(file);
            std::cout << "Last modified: " << std::ctime(&last_write_time);
            break;
        }
        case Command::PERMISSIONS: {
            fs::perms perms = fs::status(file).permissions();
            std::string perms_str = human_readable_permissions(perms);
            std::cout << "File permissions: " << perms_str << "\n";
            break;
        }
        case Command::ALL: {
            execute_command(Command::SIZE, file, size_value);
            execute_command(Command::LAST_MODIFIED, file);
            execute_command(Command::PERMISSIONS, file);
            break;
        }
        default:
            throw std::runtime_error("Unknown command");
    }
}

boost::optional<Command> get_command(const std::string& option) {
    static const std::unordered_map<std::string, Command> command_map = {
        {"size", Command::SIZE},
        {"last-modified", Command::LAST_MODIFIED},
        {"permissions", Command::PERMISSIONS},
        {"all", Command::ALL}
    };
    auto it = command_map.find(option);
    return it != command_map.end() ? boost::optional<Command>(it->second) : boost::none;
}

int print_file_info(const fs::path& file_path, const po::variables_map& options_map) {
    std::cout << "\n";
    std::cout << "------------------------------------------------------\n";
    std::cout << "File: " << file_path << "\n";
    std::cout << "------------------------------------------------------\n";

    if (fs::is_directory(file_path)) {
        throw std::runtime_error("Directory path not allowed: " + file_path.string());
    }
    if (!fs::exists(file_path)) {
        throw std::runtime_error("File does not exist: " + file_path.string());
    }

    boost::optional<SizeValue> size_value;
    if (options_map.count("size")) {
        auto parsed_size = get_size_value(options_map["size"].as<std::string>());
        if (!parsed_size) {
            throw std::invalid_argument("Invalid size value: " + options_map["size"].as<std::string>());
        }
        size_value = parsed_size;
    }

    if (options_map.count("all")) {
        execute_command(Command::ALL, file_path, size_value);
        return 0;
    }

    if (options_map.count("last-modified")) {
        execute_command(Command::LAST_MODIFIED, file_path);
    }

    if (options_map.count("permissions")) {
        execute_command(Command::PERMISSIONS, file_path);
    }

    if (options_map.count("size")) {
        execute_command(Command::SIZE, file_path, size_value);
    }

    if (!options_map.count("last-modified") && !options_map.count("permissions") &&
        !options_map.count("size") && !options_map.count("all")) {
        execute_command(Command::ALL, file_path, size_value);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    try {
        po::options_description options_desc("Allowed options");
        options_desc.add_options()
            ("help,h", "Print help message")
            ("files,f", po::value<std::vector<std::string>>()->multitoken(), "Files path")
            ("size", po::value<std::string>(), "File size unit (B, KB, MB, GB)")
            ("last-modified", "Last modified date")
            ("permissions", "File permissions")
            ("all", "Execute all commands");

        po::variables_map options_map;
        po::store(po::parse_command_line(argc, argv, options_desc), options_map);
        po::notify(options_map);

        if (options_map.count("help")) {
            print_help(options_desc);
            return 0;
        }

        if (!options_map.count("files")) {
            std::cerr << "Error: --file or -f, path is required\n";
            std::cout << COMMAND_DESCRIPTIONS << "\n";
            return 1;
        }

        std::vector<std::string> file_paths = options_map["files"].as<std::vector<std::string>>();

        for (const fs::path& file_path : file_paths) {
            try {
                print_file_info(file_path, options_map);
            } catch (const std::exception& e) {
                std::cerr << "Error processing file " << file_path << ": " << e.what() << "\n";
            }
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}