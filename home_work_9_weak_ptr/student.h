//
// Created by flyon21 on 25.11.24.
//

#ifndef C_PRO_STUDENT_H_student
#define C_PRO_STUDENT_H_student
#include <iostream>
#include <memory>
#include <list>
#include <string>


class Course;

class Student : public std::enable_shared_from_this<Student> {
private:
    std::string student_name;
    std::list<std::shared_ptr<Course>> courses;

public:
    explicit Student(const std::string& name);

    static std::shared_ptr<Student> create(const std::string& name);

    void addInCourse(const std::shared_ptr<Course>& course);

    std::string getName() const;

    void showCourses() const;

    ~Student();
};
#endif //C_PRO_STUDENT_H_student
