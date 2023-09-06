//
// Created by juan diego on 9/5/23.
//

#ifndef STATIC_HASH_BUCKET_H
#define STATIC_HASH_BUCKET_H

#include <sstream>
#include <fstream>
#include <cmath>
#include <cstdint>

#include "buffer_size.h"

const unsigned long BUFFER_SIZE = get_buffer_size();

using uint32 = uint32_t;
using int64 = int64_t;


template <
        typename RecordType
> struct Bucket {
    uint32 bucket_capacity;
    uint32 num_records;

    RecordType* records;
    int64 next_bucket;

    explicit Bucket(int bucket_capacity): num_records(0), next_bucket(-1), bucket_capacity(bucket_capacity) {
        this->records = new RecordType[this->bucket_capacity];
    }

    int size_of() {
        return 2 * sizeof(uint32) + sizeof(int64) + sizeof(RecordType) * bucket_capacity;
    }

    void read(std::fstream &file) {
        char * buffer = new char[this->size_of()];
        file.read(buffer, this->size_of());

        std::stringstream ss;
        ss.write(buffer, this->size_of());
        delete [] buffer;

        ss.read((char *) &bucket_capacity, sizeof(uint32));
        ss.read((char *) &num_records, sizeof(uint32));

        for (int i = 0; i < bucket_capacity; ++i)
            ss.read((char *) &records[i], sizeof(RecordType));

        ss.read((char *) &next_bucket, sizeof(int64));
    }

    void write(std::fstream &file) {
        std::stringstream buffer;

        buffer.write((char *) &bucket_capacity, sizeof(uint32));
        buffer.write((char *) &num_records, sizeof(uint32));

        for (int i = 0; i < bucket_capacity; ++i)
            buffer.write((char *) &records[i], sizeof(RecordType));

        buffer.write((char *) &next_bucket, sizeof(int64));

        file.write(buffer.str().c_str(), this->size_of());
    }

    void push_back(const RecordType& record) {
        records[num_records++] = record;
    }
};

template <typename RecordType>
int get_expected_bucket_capacity = std::floor(
    double((BUFFER_SIZE - (2 * sizeof(uint32) + sizeof(int64)))
            / sizeof(RecordType))
            );

#endif //STATIC_HASH_BUCKET_H
