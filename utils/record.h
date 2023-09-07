//
// Created by juan diego on 9/5/23.
//

#ifndef INDEX_RECORD_H
#define INDEX_RECORD_H

#include <ostream>

const unsigned int NAME_SIZE = 11;

struct Record {
    int id;
    char name [NAME_SIZE];
    int age;

    explicit Record() = default;

    ~Record() = default;

    explicit Record(int id, const char* _name, int age): id(id), age(age), name() {
        strncpy(name, _name, NAME_SIZE - 1);
        name[NAME_SIZE - 1] = '\0';
        std::cout << name << std::endl;
    }

    friend std::ostream & operator << (std::ostream& os, const Record& student) {
        os << "(" << student.id << ", " << student.name << ", " << student.age << ")";
        return os;
    }
};

#endif //INDEX_RECORD_H
