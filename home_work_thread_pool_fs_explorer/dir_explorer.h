#pragma once
#include <filesystem>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <queue>
#include <future>
#include "thread_pool.h"

namespace fs = std::filesystem;

class DirectoryExplorer {
public:
    DirectoryExplorer(std::ostream& out, ThreadPool& p,
                      std::mutex& outMutex, std::atomic<int>& tasks)
            : output(out), pool(p), outputMutex(outMutex), activeTasks(tasks) {}

    void explore(const fs::path& root) {
        processDirectory(root, "");
    }

private:
    void processDirectory(const fs::path& dir, const std::string& prefix) {
        std::vector<fs::directory_entry> files;
        std::vector<fs::directory_entry> directories;

        // First, collect and sort all entries
        try {
            for (const auto& entry : fs::directory_iterator(dir)) {
                if (entry.is_directory()) {
                    directories.push_back(entry);
                } else {
                    files.push_back(entry);
                }
            }
        } catch (const std::exception& e) {
            std::lock_guard<std::mutex> lock(outputMutex);
            output << prefix << "Error accessing directory: " << dir << " - " << e.what() << "\n";
            return;
        }

        // Sort directories and files separately
        auto sortByName = [](const fs::directory_entry& a, const fs::directory_entry& b) {
            return a.path().filename() < b.path().filename();
        };
        std::sort(directories.begin(), directories.end(), sortByName);
        std::sort(files.begin(), files.end(), sortByName);

        // Process directories first
        for (size_t i = 0; i < directories.size(); ++i) {
            const auto& entry = directories[i];
            bool isLastDir = (i == directories.size() - 1) && files.empty();
            std::string entryPrefix = prefix + (isLastDir ? "└── " : "├── ");
            std::string nextPrefix = prefix + (isLastDir ? "    " : "│   ");

            // Print directory name
            {
                std::lock_guard<std::mutex> lock(outputMutex);
                output << entryPrefix << entry.path().filename().string() << "/\n";
            }

            // Recursively process directory contents with updated prefix
            processDirectory(entry.path(), nextPrefix);
        }

        // Then process files
        for (size_t i = 0; i < files.size(); ++i) {
            const auto& entry = files[i];
            bool isLastEntry = (i == files.size() - 1);
            std::string entryPrefix = prefix + (isLastEntry ? "└── " : "├── ");

            {
                std::lock_guard<std::mutex> lock(outputMutex);
                output << entryPrefix << entry.path().filename().string() << "\n";
            }
        }
    }

    std::mutex& outputMutex;
    std::ostream& output;
    ThreadPool& pool;
    std::atomic<int>& activeTasks;
    static constexpr int MAX_DEPTH = 100;
};

void exploreDirectory(const fs::path& dir, std::ostream& output,
                      ThreadPool& pool, std::mutex& outputMutex,
                      std::atomic<int>& activeTasks) {
    DirectoryExplorer explorer(output, pool, outputMutex, activeTasks);
    explorer.explore(dir);
}