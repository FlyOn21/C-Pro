//
// Created by flyon21 on 14.12.24.
//
#include "task_controller.h"

TaskController::TaskController(TaskManager& model, TaskView& view) : model(model), view(view) {}

void TaskController::addTask() {
    std::string title = view.getInput("Enter task title: ");
    std::string description = view.getInput("Enter task description: ");
    TaskPriority priority = static_cast<TaskPriority>(view.getIntegerInput("Enter task priority (0: LOW, 1: MEDIUM, 2: HIGH): "));
    timedata deadline = std::chrono::system_clock::now() + std::chrono::hours(view.getIntegerInput("Enter deadline in hours: "));
    Task newTask(title, description, priority, deadline);
    model.addTask(newTask);
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
}

void TaskController::updateTask() {
    int id = view.getIntegerInput("Enter task ID to update: ");
    model.updateTask(id, [&](Task& task) {
        std::string newDescription = view.getInput("Enter new description: ");
        if (!newDescription.empty()) {
            task.setDescription(newDescription);
        }
        TaskPriority newPriority = static_cast<TaskPriority>(view.getIntegerInput("Enter new priority (0: LOW, 1: MEDIUM, 2: HIGH): "));
        task.setPriority(newPriority);
    });
    displayTasks();
}

void TaskController::removeTask() {
    int id = view.getIntegerInput("Enter task ID to remove: ");
    model.removeTask(id);
    displayTasks();
}

void TaskController::markTaskAsDone() {
    int id = view.getIntegerInput("Enter task ID to mark as done: ");
    model.updateTask(id, [](Task& task) { task.setStatus(TaskStatus::DONE); });
    displayTasks();
}