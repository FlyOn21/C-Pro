//
// Created by flyon21 on 14.12.24.
//
#include "task_viewer.h"
#include <iostream>
#include <iomanip>

void TaskView::displayTasks(const std::vector<Task>& tasks) const {
    auto format_time = [](std::chrono::system_clock::time_point time) {
        auto t = std::chrono::system_clock::to_time_t(time);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
        return oss.str();
    };
    if (tasks.empty()) {
        std::cout << "No tasks found\n";
        return;
    }
    for (const auto& task : tasks) {
        std::cout << "ID: " << task.getId() << "\n"
                  << "Title: " << task.getTitle() << "\n"
                  << "Description: " << task.getDescription() << "\n"
                  << "Priority: " << taskPriorityToString(task.getPriority()) << "\n"
                  << "Status: " << taskStatusToString(task.getStatus()) << "\n"
                  << "Deadline: " << format_time(task.getDeadline()) << "\n"
                  << "Created At: " << format_time(task.getCreatedAt()) << "\n"
                  << "Updated At: " << format_time(task.getUpdatedAt()) << "\n"
                  << "-----------------------------\n";
    }
}

std::string TaskView::getInput(const std::string& prompt) const {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int TaskView::getIntegerInput(const std::string& prompt) const {
    int input;
    while (true) {
        std::cout << prompt;
        std::cin >> input;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid integer.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return input;
        }
    }
}

void TaskView::displayError(const std::string& error) const {
    std::cerr << "Error: " << error << "\n";
}