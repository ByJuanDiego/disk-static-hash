//
// Created by juan diego on 9/5/23.
//

#ifndef INDEX_RECORD_H
#define INDEX_RECORD_H

#include <ostream>

struct Record {
    int id;
    char name [11];
    int age;

    explicit Record() = default;

    explicit Record(int id, const char* name, int age): id(id), age(age), name() {
        strcpy(this->name, name);
    }

    friend std::ostream & operator << (std::ostream& os, const Record& student) {
        os << "(" << student.id << ", " << student.name << ", " << student.age << ")";
        return os;
    }
};

#endif //INDEX_RECORD_H
