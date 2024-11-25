//
// Created by flyon21 on 25.11.24.
//
#include "student.h"
#include "course.h"

#include <algorithm>

Student::Student(const std::string& name) : student_name(name) {}

std::shared_ptr<Student> Student::create(const std::string& name) {
    return std::make_shared<Student>(name);
}

void Student::addInCourse(const std::shared_ptr<Course>& course) {
    auto it = std::find(courses.begin(), courses.end(), course);
    if (it != courses.end()) {
        std::cout << "Student " << student_name << " is already added in the course " << course->getName() << ".\n";
    } else {
        courses.push_back(course);
        course->addStudent(shared_from_this());
    }
}

std::string Student::getName() const {
    return student_name;
}

void Student::showCourses() const {
    std::cout << "Student: " << student_name << " is added in the following courses:\n";
    for (const auto& course : courses) {
        std::cout << "  - " << course->getName() << "\n";
    }
}

Student::~Student() {
    std::cout << "Student " << student_name << " is being destroyed.\n";
}