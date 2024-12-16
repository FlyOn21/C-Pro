//
// Created by flyon21 on 09.12.24.
//
#include "task_manager.h"
#include "task_viewer.h"
#include "task_controller.h"
#include <limits>

int main() {
    TaskManager manager;
    TaskView view;
    TaskController controller(manager, view);

    bool running = true;
    while (running) {
        std::cout << "Menu:\n";
        std::cout << "1. Add Task\n";
        std::cout << "2. Display Tasks\n";
        std::cout << "3. Sort Tasks\n";
        std::cout << "4. Filter Tasks\n";
        std::cout << "5. Update Task\n";
        std::cout << "6. Remove Task\n";
        std::cout << "7. Mark Task as Done\n";
        std::cout << "0. Exit\n";

        int choice;
        std::cout << "Choose an option: ";
        while (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter an integer: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                controller.addTask();
                break;
            case 2:
                controller.displayTasks();
                break;
            case 3:
                controller.sortTasks();
                break;
            case 4:
                controller.filterTasks();
                break;
            case 5:
                controller.updateTask();
                break;
            case 6:
                controller.removeTask();
                break;
            case 7:
                controller.markTaskAsDone();
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}

