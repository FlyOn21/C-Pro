#include <filesystem>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include "thread_pool.h"

namespace fs = std::filesystem;

void exploreDirectory(const fs::path& dir, std::ostream& output, ThreadPool& pool, std::mutex& outputMutex, std::atomic<int>& activeTasks, int depth = 0, std::string prefix = "") {
    constexpr int MAX_DEPTH = 100;
    if (depth > MAX_DEPTH) {
        std::lock_guard<std::mutex> lock(outputMutex);
        output << prefix << "Maximum depth reached at: " << dir << "\n";
        return;
    }

    std::vector<fs::directory_entry> entries;
    try {
        for (const auto& entry : fs::directory_iterator(dir)) {
            entries.push_back(entry);
        }
    } catch (const std::exception& e) {
        std::lock_guard<std::mutex> lock(outputMutex);
        output << prefix << "Error accessing directory: " << dir << " - " << e.what() << "\n";
        return;
    }

    {
        std::lock_guard<std::mutex> lock(outputMutex);
        if (depth > 0) {
            output << prefix << dir.filename() << "/\n";
        }
    }

    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        bool isLastEntry = (i == entries.size() - 1);

        std::string entryPrefix = prefix + (isLastEntry ? "└── " : "├── ");
        std::string childPrefix = prefix + "    ";

        if (entry.is_directory()) {
            activeTasks++;
            pool.enqueue([entry, &output, &pool, &outputMutex, &activeTasks, depth, childPrefix]() {
                exploreDirectory(entry.path(), output, pool, outputMutex, activeTasks, depth + 1, childPrefix);
                activeTasks--;
            });
        } else {
            {
                std::lock_guard<std::mutex> lock(outputMutex);
                output << entryPrefix << entry.path().filename() << "\n";
            }
        }
    }

    activeTasks--;
}