//
// Created by flyon21 on 14.12.24.
//
#include "task_manager.h"
#include <algorithm>

bool TaskManager::addTask(const Task& task) {
    try{
        tasks.push_back(task);
        return true;
    }
    catch (const std::exception& e){
        return false;
    }
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

bool TaskManager::updateTask(int id, std::function<void(Task&)> updater) {
    for (auto& task : tasks) {
        if (task.getId() == id) {
            updater(task);
            return true;
        }
    }
    return false;
}

bool TaskManager::removeTask(int id) {
    auto original_size = tasks.size();
    tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
                               [id](const Task& task) { return task.getId() == id; }),
                tasks.end());
    return tasks.size() < original_size;
}