//
// Created by juan diego on 9/5/23.
//

#ifndef INDEX_RECORD_H
#define INDEX_RECORD_H

#include <ostream>

struct Record {
    int id;
    char name [10];
    int age;

    friend std::ostream & operator << (std::ostream& os, const Record& student) {
        os << "(" << student.id << ", " << student.name << ", " << student.age << ")";
        return os;
    }
};

#endif //INDEX_RECORD_H
