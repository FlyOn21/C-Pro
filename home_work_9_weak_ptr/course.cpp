//
// Created by flyon21 on 25.11.24.
//
#include "course.h"
#include "student.h"


Course::Course(const std::string& name) : course_name(name) {}

void Course::addStudent(const std::shared_ptr<Student>& student) {
    for (const auto& weak_student : students) {
        if (auto s = weak_student.lock()) {
            if (s == student) {
                std::cout << "Student " << student->getName() << " is already added in the course " << course_name << ".\n";
                return;
            }
        }
    }
    students.push_back(student);
}

std::string Course::getName() const {
    return course_name;
}

void Course::showStudents() const {
    std::cout << "Course: " << course_name << " has the following students:\n";
    if (students.empty()) {
        std::cout << "  - No students\n";
        return;
    }
    for (const auto& weak_student : students) {
        if (auto student = weak_student.lock()) {
            std::cout << "  - " << student->getName() << "\n";
        } else {
            std::cout << "  - [Expired Student]\n";
        }
    }
}

Course::~Course() {
    std::cout << "Course " << course_name << " is being destroyed.\n";
}