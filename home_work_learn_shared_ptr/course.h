//
// Created by flyon21 on 18.11.24.
//

#ifndef C_PRO_COURSE_H_course
#define C_PRO_COURSE_H_course

#include <cstring>
#include <list>
#include "students.h"

class Course {
public:

    Course(const char* courseName) {
        static int c_id = 0;
        this->course_id = ++c_id;
        std::strncpy(this->courseName, courseName, sizeof(this->courseName) - 1);
        this->courseName[sizeof(this->courseName) - 1] = '\0';
    }

    void add_student(Student* student) {
        students.push_back(student);
    }
    void remove_student(Student* student) {
        students.remove(student);
    }
    int get_id() const {
        return course_id;
    }
    const char* get_name() const {
        return courseName;
    }
    const std::list<Student*>& get_students() const {
        return students;
    }

private:
    int course_id;
    char courseName[256];
    std::list<Student*> students;
};



#endif //C_PRO_COURSE_H_course
