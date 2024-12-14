//
// Created by flyon21 on 14.12.24.
//

#ifndef C_PRO_TASK_MANAGER_H_task_manager
#define C_PRO_TASK_MANAGER_H_task_manager

#include "task.h"
#include <vector>
#include <functional>

class TaskManager {
private:
    std::vector<Task> tasks;

public:
    bool addTask(const Task& task);
    std::vector<Task> getTasks() const;
    std::vector<Task> filterTasks(std::function<bool(const Task&)> filter) const;
    void sortTasks(std::function<bool(const Task&, const Task&)> comparator);
    bool updateTask(int id, std::function<void(Task&)> updater);
    bool removeTask(int id);
};

#endif //C_PRO_TASK_MANAGER_H_task_manager
