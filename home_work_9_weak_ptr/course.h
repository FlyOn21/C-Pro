//
// Created by flyon21 on 25.11.24.
//

#ifndef C_PRO_COURSE_H_course
#define C_PRO_COURSE_H_course

#include <iostream>
#include <memory>
#include <list>
#include <string>


class Student;

class Course {
private:
    std::string course_name;
    std::list<std::weak_ptr<Student>> students;




public:
    explicit Course(const std::string& name);

    void addStudent(const std::shared_ptr<Student>& student);

    std::string getName() const;

    void showStudents() ;

    ~Course();

    void cleanup_expired() {
        students.remove_if([](const std::weak_ptr<Student>& wp) {
            return wp.expired();
        });
    };
};
#endif //C_PRO_COURSE_H_course
