//
// Created by flyon21 on 14.12.24.
//

#ifndef C_PRO_TASK_CONTROLLER_H_task_controller
#define C_PRO_TASK_CONTROLLER_H_task_controller

#include "task_manager.h"
#include "task_viewer.h"

using timedata = std::chrono::system_clock::time_point;

class TaskController {
private:
    TaskManager& model;
    TaskView& view;

public:
    TaskController(TaskManager& model, TaskView& view);
    void addTask();
    void displayTasks();
    void sortTasks();
    void filterTasks();
    void updateTask();
    void removeTask();
    void markTaskAsDone();
};

#endif //C_PRO_TASK_CONTROLLER_H_task_controller
