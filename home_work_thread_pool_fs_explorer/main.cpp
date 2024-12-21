#include "dir_explorer.h"
#include <atomic>
#include <iostream>

int main() {
    std::string directoryPath;
    std::cout << "Enter the path to the directory: ";
    std::getline(std::cin, directoryPath);
    fs::path root(directoryPath);

    if (!fs::exists(root) || !fs::is_directory(root)) {
        std::cerr << "Invalid root directory." << std::endl;
        return 1;
    }

    std::stringstream output;
    std::mutex outputMutex;
    std::atomic<int> activeTasks(0);
    ThreadPool pool(std::thread::hardware_concurrency());

    output << "Starting directory traversal...\n";
    {
        std::lock_guard<std::mutex> lock(outputMutex);
        output << root.filename() << "/\n";
    }

    activeTasks++;
    exploreDirectory(root, output, pool, outputMutex, activeTasks);

    while (activeTasks > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << output.str();
    }

    return 0;
}