//
// Created by flyon21 on 18.11.24.
//

#include <iostream>
#include "course.h"
#include "students.h"
#include <vector>
#include <memory>

void print_courses(const std::vector<std::shared_ptr<Course>>& courses) {
    for (const auto& course : courses) {
        std::cout << "Course ID: " << course->get_id()
                  << ", Name: " << course->get_name() << std::endl;
        std::cout << "Students:" << std::endl;

        for (const auto& student : course->get_students()) {
            std::cout << " - ID: " << student->student_id
                      << ", Name: " << student->name
                      << ", Group: " << student->group << std::endl;
        }
        std::cout << "------------------------------------------------------" << std::endl;
    }
}

int main() {

    std::vector<std::shared_ptr<Student>> students;
    std::vector<std::shared_ptr<Course>> courses;

    students.push_back(std::make_shared<Student>("Zhoholiev", "A1"));
    students.push_back(std::make_shared<Student>("Ivanov", "B2"));
    students.push_back(std::make_shared<Student>("Doy", "A1"));
    students.push_back(std::make_shared<Student>("Petrenko", "C3"));

    auto math = std::make_shared<Course>("Math");

    courses.push_back(math);

    for (auto& student : students) {
        math->add_student(student);
    }

    print_courses(courses);

    math->remove_student(students[0]);
    math->remove_student(students[1]);

    print_courses(courses);


}