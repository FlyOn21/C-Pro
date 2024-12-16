//
// Created by flyon21 on 14.12.24.
//

#ifndef C_PRO_TASK_VIEWER_H_task_viewer
#define C_PRO_TASK_VIEWER_H_task_viewer

#include "task.h"
#include <vector>
#include <string>

class TaskView {
public:
    void displayTasks(const std::vector<Task>& tasks) const;
    std::string getInput(const std::string& prompt) const;
    int getIntegerInput(const std::string& prompt) const;
    void displayError(const std::string& error) const;
};
#endif //C_PRO_TASK_VIEWER_H_task_viewer
