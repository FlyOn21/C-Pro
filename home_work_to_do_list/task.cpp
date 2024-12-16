//
// Created by flyon21 on 14.12.24.
//
#include "task.h"

std::atomic<int> Task::global_id{1};

Task::Task(const std::string& title, const std::string& description, TaskPriority priority, std::chrono::system_clock::time_point deadline)
        : id(global_id++), title(title), description(description), priority(priority), status(TaskStatus::NEW),
          deadline(deadline), created_at(std::chrono::system_clock::now()), updated_at(std::chrono::system_clock::now()) {}