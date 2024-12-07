//
// Created by flyon21 on 02.12.24.
//

#ifndef C_PRO_FILESYSTEM_VIEWER_H_FILESYSTEM_VIEWER_H
#define C_PRO_FILESYSTEM_VIEWER_H_FILESYSTEM_VIEWER_H

#include <iostream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;

struct TreeSymbols {
    const std::string branch = "├── ";
    const std::string empty = "    ";
};

struct FileStats {
    size_t count = 0;
    uintmax_t totalSize = 0;

    void add(uintmax_t size) {
        count++;
        totalSize += size;
    }
};

struct FileAnalysis {
    FileStats text;
    FileStats images;
    FileStats executables;
    FileStats others;
};

class FileSystemViewer {
public:
    explicit FileSystemViewer(const std::string &path) : path(fs::canonical(path)) {
        if (!fs::exists(path)) {
            throw std::invalid_argument("Path does not exist");
        }
    }

    void show() {
        try {
            if (!fs::exists(path)) {
                std::cerr << "Error: The specified path does not exist." << std::endl;
                return;
            }

            for (fs::recursive_directory_iterator it(path, fs::directory_options::skip_permission_denied), end;
                 it != end; ++it) {
                try {
                    const auto &entry = *it;

                    while (stack.size() > it.depth()) {
                        stack.pop_back();
                    }
                    if (!stack.empty()) {
                        prefix = stack.back();
                    }
                    std::cout << prefix
                              <<  symbols.branch
                              << entry.path().filename().generic_string();

                    if (fs::is_directory(entry)) {
                        if ((fs::status(entry).permissions() & fs::perms::owner_read) != fs::perms::none) {
                            std::cout << " [DIR]" << std::endl;

                            stack.push_back(prefix +  symbols.empty);
                        } else {
                            std::cout << " (permission denied) [DIR]" << std::endl;
                        }
                    } else if (fs::is_regular_file(entry)) {
                        std::cout << " [FILE]" << std::endl;
                    } else if (fs::is_symlink(entry)) {
                        std::cout << " [SYMLINK]" << std::endl;
                    } else {
                        std::cout << " [OTHER]" << std::endl;
                    }

                } catch (const fs::filesystem_error &) {
                    prefix = stack.empty() ? "" : stack.back();
                    std::cerr << prefix
                              << symbols.branch
                              << it->path().filename().string()
                              << " (permission denied)" << std::endl;
                }
            }
        } catch (const fs::filesystem_error &e) {
            std::cerr << "Error iterating directory: " << e.what() << std::endl;
        }
    }


    void analyzeFiles() {

        try {
            for (const auto& entry : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied)) {
                if (!fs::is_regular_file(entry)) continue;

                try {
                    auto& stats = categorizeFile(entry);
                    stats.add(fs::file_size(entry));
                } catch (const fs::filesystem_error&) {
                    continue;
                }
            }

            printAnalysis();
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error analyzing files: " << e.what() << std::endl;
        }
    }

private:
    std::string path;
    TreeSymbols symbols;
    std::string prefix;
    int lastDepth = -1;
    std::vector<std::string> stack;
    FileAnalysis analysis;

    FileStats& categorizeFile(const fs::directory_entry& entry) {
        auto ext = entry.path().extension().string();
        if (ext == ".txt") return analysis.text;
        if (ext == ".jpg" || ext == ".png" || ext == ".bmp") return analysis.images;
        if (isExecutable(entry)) return analysis.executables;
        return analysis.others;
    }

    void printAnalysis() {
        std::cout << "\nAnalysis of directory: " << path << std::endl
                  << "Text files (.txt): " << analysis.text.count
                  << " (" << formatSize(analysis.text.totalSize) << ")\n"
                  << "Image files (.jpg, .png, .bmp): " << analysis.images.count
                  << " (" << formatSize(analysis.images.totalSize) << ")\n"
                  << "Executable files: " << analysis.executables.count
                  << " (" << formatSize(analysis.executables.totalSize) << ")\n"
                  << "Other files: " << analysis.others.count
                  << " (" << formatSize(analysis.others.totalSize) << ")" << std::endl;
    }

    bool isExecutable(const fs::directory_entry &entry) {
        #ifdef _WIN32
            auto ext = entry.path().extension().string();
            return ext == ".exe" || ext == ".bat" || ext == ".cmd";
        #else
                auto perms = fs::status(entry).permissions();
                return (perms & fs::perms::owner_exec) != fs::perms::none ||
                       (perms & fs::perms::group_exec) != fs::perms::none ||
                       (perms & fs::perms::others_exec) != fs::perms::none;
        #endif
    }

    std::string formatSize(uintmax_t size) const {
        const char *suffix[] = {"B", "KB", "MB", "GB", "TB"};
        int i = 0;
        double doubleSize = static_cast<double>(size);

        while (doubleSize >= 1024 && i < 4) {
            doubleSize /= 1024;
            ++i;
        }

        char buffer[50];
        std::snprintf(buffer, sizeof(buffer), "%.2f %s", doubleSize, suffix[i]);
        return std::string(buffer);
    }
};


#endif //C_PRO_FILESYSTEM_VIEWER_H_FILESYSTEM_VIEWER_H
