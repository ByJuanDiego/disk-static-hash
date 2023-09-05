//
// Created by juan diego on 9/5/23.
//

#ifndef STATIC_HASH_BUCKET_H
#define STATIC_HASH_BUCKET_H

#include "dependencies.h"

template <
        typename RecordType
> struct Bucket {
    types::uint32 bucket_capacity;
    types::uint32 num_records;

    RecordType* records;
    types::int64 next_bucket;

    explicit Bucket(int blocking_factor) : num_records(0), next_bucket(-1) {
        bucket_capacity = blocking_factor;
        this->records = new RecordType[this->bucket_capacity];
    }

    int size_of() {
        return 2 * constants::size_of<types::uint32> + constants::size_of<RecordType> * bucket_capacity;
    }

    void read(std::fstream &file) {
        char * buffer = new char[this->size_of()];
        file.read(buffer, this->size_of());

        std::stringstream ss;
        ss.write(buffer, this->size_of());
        delete [] buffer;

        ss.read((char *) &bucket_capacity, constants::size_of<types::uint32>);
        ss.read((char *) &num_records, constants::size_of<types::uint32>);

        for (int i = 0; i < bucket_capacity; ++i)
            ss.read((char *) &records[i], constants::size_of<RecordType>);

        ss.read((char *) &next_bucket, constants::size_of<types::int64>);
    }


    void write(std::fstream &file) {
        std::stringstream buffer;

        buffer.write((char *) &bucket_capacity, constants::size_of<types::uint32>);
        buffer.write((char *) &num_records, constants::size_of<types::uint32>);

        for (int i = 0; i < bucket_capacity; ++i)
            buffer.write((char *) &records[i], constants::size_of<RecordType>);

        buffer.write((char *) &next_bucket, constants::size_of<types::int64>);

        file.write(buffer.str().c_str(), this->size_of());
    }
};

template <typename RecordType>
int get_expected_bucket_capacity = std::floor(
    double(constants::buffer_size - (2 *  constants::size_of<types::uint32> + constants::size_of<types::int64>))
            / constants::size_of<RecordType>
);

#endif //STATIC_HASH_BUCKET_H
