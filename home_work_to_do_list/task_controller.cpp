//
// Created by flyon21 on 14.12.24.
//
#include "task_controller.h"

TaskController::TaskController(TaskManager& model, TaskView& view) : model(model), view(view) {}

void TaskController::addTask() {
    std::string title = view.getInput("Enter task title: ");
    std::string description = view.getInput("Enter task description: ");

    TaskPriority priority;
    while (true) {
        std::string priorityInput = view.getInput("Enter task priority (0: LOW, 1: MEDIUM, 2: HIGH): ");
        try {
            int priorityValue = std::stoi(priorityInput);
            if (priorityValue >= 0 && priorityValue <= 2) {
                priority = static_cast<TaskPriority>(priorityValue);
                break;
            } else {
                view.displayError("Invalid priority. Please enter 0 for LOW, 1 for MEDIUM, or 2 for HIGH.");
            }
        } catch (const std::invalid_argument&) {
            view.displayError("Invalid input. Please enter a valid integer for priority.");
        } catch (const std::out_of_range&) {
            view.displayError("Input out of range. Please enter a valid integer for priority.");
        }
    }

    int deadlineInput;
    while (true) {
        std::string deadlineStr = view.getInput("Enter deadline in hours: ");
        try {
            int deadlineValue = std::stoi(deadlineStr);
            if (deadlineValue > 0) {
                deadlineInput = deadlineValue;
                break;
            } else {
                view.displayError("Invalid deadline. Please enter a positive number of hours.");
            }
        } catch (const std::invalid_argument&) {
            view.displayError("Invalid input. Please enter a valid integer for deadline.");
        } catch (const std::out_of_range&) {
            view.displayError("Input out of range. Please enter a valid integer for deadline.");
        }
    }

    timedata deadline = std::chrono::system_clock::now() + std::chrono::hours(deadlineInput);

    Task newTask(title, description, priority, deadline);
    bool isAdded = model.addTask(newTask);
    if (!isAdded) {
        view.displayError("Failed to add task");
        return;
    }
    displayTasks();
}

void TaskController::displayTasks() {
    view.displayTasks(model.getTasks());
}

void TaskController::sortTasks() {
    int choice = view.getIntegerInput("Sort by: 1. Priority, 2. Deadline: ");
    if (choice == 1) {
        model.sortTasks([](const Task& a, const Task& b) { return static_cast<int>(a.getPriority()) > static_cast<int>(b.getPriority()); });
    } else if (choice == 2) {
        model.sortTasks([](const Task& a, const Task& b) { return a.getDeadline() < b.getDeadline(); });
    }
    else {
        view.displayError("Invalid choice");
        return;
    }
    displayTasks();
}

void TaskController::filterTasks() {
    int choice = view.getIntegerInput("Filter by: 1. Done, 2. Priority: ");
    if (choice == 1) {
        auto filtered = model.filterTasks([](const Task& task) { return task.getStatus() == TaskStatus::DONE; });
        view.displayTasks(filtered);
    } else if (choice == 2) {
        TaskPriority priority = static_cast<TaskPriority>(view.getIntegerInput("Enter priority (0: LOW, 1: MEDIUM, 2: HIGH): "));
        auto filtered = model.filterTasks([priority](const Task& task) { return task.getPriority() == priority; });
        view.displayTasks(filtered);
    }
    else {
        view.displayError("Invalid choice");
        return;
    }
}

void TaskController::updateTask() {
    int id = view.getIntegerInput("Enter task ID to update: ");
    bool isUpdated = model.updateTask(id, [&](Task& task) {
        std::string newDescription = view.getInput("Enter new description: ");
        if (!newDescription.empty()) {
            task.setDescription(newDescription);
        }
        TaskPriority newPriority = static_cast<TaskPriority>(view.getIntegerInput("Enter new priority (0: LOW, 1: MEDIUM, 2: HIGH): "));
        task.setPriority(newPriority);
    });
    if (!isUpdated) {
        view.displayError("Task not found");
    }
    else {
        view.displayTasks(model.getTasks());
    }
}

void TaskController::removeTask() {
    int id = view.getIntegerInput("Enter task ID to remove: ");
    bool isDeleted = model.removeTask(id);
    if (!isDeleted) {
        view.displayError("Task not found");
        return;
    }
    displayTasks();
}

void TaskController::markTaskAsDone() {
    int id = view.getIntegerInput("Enter task ID to mark as done: ");
    bool isDone = model.updateTask(id, [](Task& task) { task.setStatus(TaskStatus::DONE); });
    if (!isDone) {
        view.displayError("Task not found");
        return;
    }
    displayTasks();
}