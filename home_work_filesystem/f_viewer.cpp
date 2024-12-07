//
// Created by flyon21 on 02.12.24.
//

#include "filesystem_viewer.h"

int main() {
    std::string directoryPath;

    std::cout << "Enter the path to the directory: ";
    std::getline(std::cin, directoryPath);

    fs::path path(directoryPath);
    if (fs::exists(path) && fs::is_directory(path)) {
        std::cout << "Directory exists: " << path << std::endl;
    } else {
        std::cerr << "Invalid directory path or directory does not exist." << std::endl;
    }
    FileSystemViewer viewer(path);
    viewer.show();
    viewer.analyzeFiles();
    return 0;
}