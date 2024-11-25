//
// Created by flyon21 on 25.11.24.
//
#include "student.h"
#include "course.h"

int main() {
    auto pasha = std::make_shared<Student>("Pasha");
    auto natasha = std::make_shared<Student>("Natasha");

    auto math = std::make_shared<Course>("Math");
    auto physics = std::make_shared<Course>("Physics");

    math->showStudents();
    physics->showStudents();

    pasha->addInCourse(math);
    natasha->addInCourse(physics);
    pasha->addInCourse(physics);

    math->showStudents();
    physics->showStudents();

    natasha->addInCourse(math);
    natasha->addInCourse(math);

    natasha->showCourses();
    pasha->showCourses();

    return 0;
}
