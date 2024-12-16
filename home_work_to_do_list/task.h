//
// Created by flyon21 on 09.12.24.
//

#ifndef C_PRO_TASK_H_task_entity
#define C_PRO_TASK_H_task_entity
#include <string>
#include <iostream>
#include <atomic>
#include <chrono>
#include <unordered_map>

enum class TaskStatus {
    NEW,
    IN_PROGRESS,
    TESTING,
    DEADLINE_EXCEEDED,
    DONE
};

enum class TaskPriority {
    LOW,
    MEDIUM,
    HIGH
};

inline std::string taskStatusToString(TaskStatus status) {
    switch (status) {
        case TaskStatus::NEW: return "NEW";
        case TaskStatus::IN_PROGRESS: return "IN_PROGRESS";
        case TaskStatus::TESTING: return "TESTING";
        case TaskStatus::DEADLINE_EXCEEDED: return "DEADLINE_EXCEEDED";
        case TaskStatus::DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

inline std::string taskPriorityToString(TaskPriority priority) {
    switch (priority) {
        case TaskPriority::LOW: return "LOW";
        case TaskPriority::MEDIUM: return "MEDIUM";
        case TaskPriority::HIGH: return "HIGH";
        default: return "UNKNOWN";
    }
}

class Task {
private:
    static std::atomic<int> global_id;
    int id;
    std::string title;
    std::string description;
    TaskPriority priority;
    TaskStatus status;
    std::chrono::system_clock::time_point deadline;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;

public:
    Task(const std::string& title, const std::string& description, TaskPriority priority, std::chrono::system_clock::time_point deadline);


    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getDescription() const { return description; }
    TaskPriority getPriority() const { return priority; }
    TaskStatus getStatus() const { return status; }
    std::chrono::system_clock::time_point getCreatedAt() const { return created_at; }
    std::chrono::system_clock::time_point getUpdatedAt() const { return updated_at; }
    std::chrono::system_clock::time_point getDeadline() const { return deadline; }


    void setDescription(const std::string& newDescription) {
        description = newDescription;
        updated_at = std::chrono::system_clock::now();
    }
    void setPriority(TaskPriority newPriority) {
        priority = newPriority;
        updated_at = std::chrono::system_clock::now();
    }
    void setStatus(TaskStatus newStatus) {
        status = newStatus;
        updated_at = std::chrono::system_clock::now();
    }
};

#endif //C_PRO_TASK_H_task_entity
