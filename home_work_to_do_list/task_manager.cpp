//
// Created by flyon21 on 14.12.24.
//
#include "task_manager.h"
#include <algorithm>

void TaskManager::addTask(const Task& task) {
    tasks.push_back(task);
}

std::vector<Task> TaskManager::getTasks() const {
    return tasks;
}

std::vector<Task> TaskManager::filterTasks(std::function<bool(const Task&)> filter) const {
    std::vector<Task> result;
    for (const auto& task : tasks) {
        if (filter(task)) {
            result.push_back(task);
        }
    }
    return result;
}

void TaskManager::sortTasks(std::function<bool(const Task&, const Task&)> comparator) {
    std::sort(tasks.begin(), tasks.end(), comparator);
}

void TaskManager::updateTask(int id, std::function<void(Task&)> updater) {
    for (auto& task : tasks) {
        if (task.getId() == id) {
            updater(task);
            break;
        }
    }
}

void TaskManager::removeTask(int id) {
    tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
                               [id](const Task& task) { return task.getId() == id; }),
                tasks.end());
}