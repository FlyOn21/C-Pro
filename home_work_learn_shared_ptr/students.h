//
// Created by flyon21 on 18.11.24.
//

#ifndef C_PRO_STUDENTS_H_stud
#define C_PRO_STUDENTS_H_stud
#include <cstring>

struct Student {
    int student_id;
    char name[256];
    char group[120];

    Student(const char* name, const char* group) {
        static int stud_id = 0;
        this->student_id = ++stud_id;
        std::strncpy(this->name, name, sizeof(this->name) - 1);
        this->name[sizeof(this->name) - 1] = '\0';
        std::strncpy(this->group, group, sizeof(this->group) - 1);
        this->group[sizeof(this->group) - 1] = '\0';
    }
};

#endif
