//
// Created by juan diego on 9/4/23.
//

#ifndef STATIC_HASH_STATIC_HASH_H
#define STATIC_HASH_STATIC_HASH_H

#include <functional>
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>

#include "bucket.h"
#include "property.h"
#include "file_utils.h"

#include <iostream>

const std::string KEY_NOT_FOUND = "Key not found";
const std::string REPEATED_KEY = "Repeated key";
const std::string CREATE_DIR_ERROR = "Error creating directory";

template <
    typename KeyType,
    typename RecordType,
    typename Equal = std::equal_to<KeyType>,
    typename Hash = std::hash<KeyType>,
    typename Index = std::function<KeyType(RecordType)>
>
struct StaticHash {
private:

    std::fstream metadata_file;
    std::fstream static_hash_file;

    Json::Value metadata_json;

    Hash hash_function;
    Equal equal_function;
    Index get_indexed_field;

    void load_metadata() {
        metadata_file >> metadata_json;
    }

    void initialize_new_index() {
        if (!directory_exists(metadata_json[DIRECTORY_PATH].asString())) {
            bool successfully_created = create_directory(metadata_json[DIRECTORY_PATH].asString());
            if (!successfully_created) {
                throw std::runtime_error(CREATE_DIR_ERROR);
            }
        }

        metadata_file.open(metadata_json[METADATA_FULL_PATH].asString(), std::ios::out);
        metadata_file << metadata_json;
        metadata_file.close();

        static_hash_file.open(metadata_json[INDEX_FULL_PATH].asString(), std::ios::out);
        for (int i = 0; i < metadata_json[HASH_TABLE_SIZE].asInt(); ++i) {
            Bucket<RecordType> bucket(metadata_json[BUCKET_CAPACITY].asInt());
            bucket.write(static_hash_file);
        }
        static_hash_file.close();
    }

public:

    explicit StaticHash(const Property& property, Index index, Hash hash = Hash(), Equal equal = Equal())
    :   get_indexed_field(index),
        hash_function(hash),
        equal_function(equal) {
        metadata_json = property.json_value();
        metadata_file.open(metadata_json[METADATA_FULL_PATH].asString(), std::ios::in);

        if (metadata_file.good()) {
            load_metadata();
        } else {
            metadata_file.close();
            initialize_new_index();
        }
        metadata_file.close();
    }

    std::vector<RecordType> search(KeyType key) {
        int i = static_cast<int>(hash_function(key) % metadata_json[HASH_TABLE_SIZE].asInt());
        Bucket<RecordType> bucket(metadata_json[BUCKET_CAPACITY].asInt());

        long long seek_bucket = i * bucket.size_of();
        static_hash_file.open(metadata_json[INDEX_FULL_PATH].asString(), std::ios::in | std::ios::binary);

        std::vector<RecordType> located_records;
        bool any_found = false;

        do {
            static_hash_file.seekg(seek_bucket);
            bucket.read(static_hash_file);

            for (int j = 0; j < bucket.num_records; ++j) {
                if (!equal_function(key, get_indexed_field(bucket.records[j]))) {
                    continue;
                }

                any_found = true;
                located_records.push_back(bucket.records[j]);

                if (!metadata_json[PRIMARY_KEY].asBool()) {
                    continue;
                }

                static_hash_file.close();
                return located_records;
            }

            seek_bucket = bucket.next_bucket;
        } while (seek_bucket != -1);

        static_hash_file.close();
        if (any_found) {
            return located_records;
        }
        throw std::runtime_error(KEY_NOT_FOUND);
    }

    void insert(const RecordType& record) {
        int i = static_cast<int>(hash_function(get_indexed_field(record)) % metadata_json[HASH_TABLE_SIZE].asInt());
        Bucket<RecordType> bucket(metadata_json[BUCKET_CAPACITY].asInt());

        long long seek_bucket = i * bucket.size_of();
        static_hash_file.open(metadata_json[INDEX_FULL_PATH].asString(), std::ios::in | std::ios::out | std::ios::binary);

        long long bucket_to_insert = -1;
        long long prev_bucket;

        do {
            static_hash_file.seekg(seek_bucket);
            bucket.read(static_hash_file);

            if (bucket.num_records < bucket.bucket_capacity) {
                if (bucket_to_insert == -1) {
                    bucket_to_insert = seek_bucket;
                }

                if (!metadata_json[PRIMARY_KEY].asBool()) {
                    break;
                }
            }

            if (metadata_json[PRIMARY_KEY].asBool()) {
                for (int j = 0; j < bucket.num_records; ++j) {
                    if (!equal_function(get_indexed_field(record), get_indexed_field(bucket.records[j]))) {
                        continue;
                    }
                    static_hash_file.close();
                    throw std::runtime_error(REPEATED_KEY);
                }
            }

            prev_bucket = seek_bucket;
            seek_bucket = bucket.next_bucket;

        } while (seek_bucket != -1);

        if (bucket_to_insert  != -1) {
            static_hash_file.seekg(bucket_to_insert);
            bucket.read(static_hash_file);
            bucket.push_back(record);
            static_hash_file.seekp(bucket_to_insert);
            bucket.write(static_hash_file);
        } else {
            Bucket<RecordType> new_bucket(metadata_json[BUCKET_CAPACITY].asInt());
            new_bucket.push_back(record);
            static_hash_file.seekp(0, std::ios::end);
            long long new_bucket_position = static_hash_file.tellg();
            new_bucket.write(static_hash_file);

            static_hash_file.seekg(prev_bucket);
            bucket.read(static_hash_file);
            bucket.next_bucket = new_bucket_position;
            static_hash_file.seekp(prev_bucket);
            bucket.write(static_hash_file);
        }

        static_hash_file.close();
    }

    void remove(KeyType key) {

    }
};

#endif //STATIC_HASH_STATIC_HASH_H
