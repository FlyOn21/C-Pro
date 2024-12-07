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

class FileSystemViewer {
public:
    explicit FileSystemViewer(const std::string &path) : path(path) {
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
                    std::string prefix = stack.empty() ? "" : stack.back();
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
        size_t txtCount = 0;
        size_t imageCount = 0;
        size_t exeCount = 0;
        size_t otherCount = 0;

        uintmax_t txtSize = 0;
        uintmax_t imageSize = 0;
        uintmax_t exeSize = 0;
        uintmax_t otherSize = 0;

        try {
            for (fs::recursive_directory_iterator it(path, fs::directory_options::skip_permission_denied), end;
                 it != end; ++it) {
                try {
                    const auto &entry = *it;

                    if (fs::is_regular_file(entry)) {
                        auto ext = entry.path().extension().string();
                        uintmax_t fileSize = fs::file_size(entry);

                        if (ext == ".txt") {
                            ++txtCount;
                            txtSize += fileSize;
                        } else if (ext == ".jpg" || ext == ".png" || ext == ".bmp") {
                            ++imageCount;
                            imageSize += fileSize;
                        } else if (isExecutable(entry)) {
                            ++exeCount;
                            exeSize += fileSize;
                        } else {
                            ++otherCount;
                            otherSize += fileSize;
                        }
                    }
                } catch (const fs::filesystem_error &) {
                    continue;
                }
            }

            std::cout << "\nAnalysis of directory: " << path << std::endl;
            std::cout << "Text files (.txt): " << txtCount << " (" << formatSize(txtSize) << ")" << std::endl;
            std::cout << "Image files (.jpg, .png, .bmp): " << imageCount << " (" << formatSize(imageSize) << ")"
                      << std::endl;
            std::cout << "Executable files: " << exeCount << " (" << formatSize(exeSize) << ")" << std::endl;
            std::cout << "Other files: " << otherCount << " (" << formatSize(otherSize) << ")" << std::endl;

        } catch (const fs::filesystem_error &e) {
            std::cerr << "Error analyzing files: " << e.what() << std::endl;
        }
    }

private:
    std::string path;
    TreeSymbols symbols;
    std::string prefix;
    int lastDepth = -1;
    std::vector<std::string> stack;

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
