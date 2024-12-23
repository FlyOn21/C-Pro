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
        {
            std::lock_guard<std::mutex> lock(outputMutex);
            output << "/\n";
        }
        processRootDirectory(root).wait();
    }

private:
    std::future<void> processRootDirectory(const fs::path& dir) {
        return pool.enqueue([this, dir]() {
            std::vector<fs::directory_entry> entries;

            try {
                for (const auto& entry : fs::directory_iterator(dir)) {
                    entries.push_back(entry);
                }
            } catch (const std::exception& e) {
                std::lock_guard<std::mutex> lock(outputMutex);
                output << "Error accessing directory: " << dir << " - " << e.what() << "\n";
                return;
            }

            std::sort(entries.begin(), entries.end(),
                      [](const fs::directory_entry& a, const fs::directory_entry& b) {
                          return a.path().filename() < b.path().filename();
                      });

            for (size_t i = 0; i < entries.size(); ++i) {
                const auto& entry = entries[i];
                bool isLast = (i == entries.size() - 1);

                {
                    std::lock_guard<std::mutex> lock(outputMutex);
                    output << (isLast ? "└── " : "├── ")
                           << entry.path().filename().string()
                           << (entry.is_directory() ? "/\n" : "\n");
                }

                if (entry.is_directory()) {
                    processSubDirectory(entry.path(), isLast ? "    " : "│   ").wait();
                }
            }
        });
    }

    std::future<void> processSubDirectory(const fs::path& dir, const std::string& parentPrefix) {
        activeTasks++;
        return pool.enqueue([this, dir, parentPrefix]() {
            std::vector<fs::directory_entry> entries;

            try {
                for (const auto& entry : fs::directory_iterator(dir)) {
                    entries.push_back(entry);
                }
            } catch (const std::exception& e) {
                std::lock_guard<std::mutex> lock(outputMutex);
                output << parentPrefix << "Error accessing directory: " << dir << " - " << e.what() << "\n";
                activeTasks--;
                return;
            }

            std::sort(entries.begin(), entries.end(),
                      [](const fs::directory_entry& a, const fs::directory_entry& b) {
                          return a.path().filename() < b.path().filename();
                      });

            for (size_t i = 0; i < entries.size(); ++i) {
                const auto& entry = entries[i];
                bool isLast = (i == entries.size() - 1);

                {
                    std::lock_guard<std::mutex> lock(outputMutex);
                    output << parentPrefix << (isLast ? "└── " : "├── ")
                           << entry.path().filename().string()
                           << (entry.is_directory() ? "/\n" : "\n");
                }

                if (entry.is_directory()) {
                    std::string newPrefix = parentPrefix + (isLast ? "    " : "│   ");
                    processSubDirectory(entry.path(), newPrefix).wait();
                }
            }

            activeTasks--;
        });
    }

    std::mutex& outputMutex;
    std::ostream& output;
    ThreadPool& pool;
    std::atomic<int>& activeTasks;
};

void exploreDirectory(const fs::path& dir, std::ostream& output,
                      ThreadPool& pool, std::mutex& outputMutex,
                      std::atomic<int>& activeTasks) {
    DirectoryExplorer explorer(output, pool, outputMutex, activeTasks);
    explorer.explore(dir);
}