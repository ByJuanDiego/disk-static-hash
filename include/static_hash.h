//
// Created by juan diego on 9/4/23.
//

#ifndef STATIC_HASH_STATIC_HASH_H
#define STATIC_HASH_STATIC_HASH_H

#include <functional>

#include "bucket.h"
#include "property.h"
#include "file_utils.h"

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
    Equal is_equal;
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
        is_equal(equal) {
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

            // iterates through the current bucket looking for records with the same key
            for (int j = 0; j < bucket.num_records; ++j) {
                if (!is_equal(key, get_indexed_field(bucket.records[j]))) {
                    continue;
                }

                any_found = true;
                located_records.push_back(bucket.records[j]);

                if (!metadata_json[PRIMARY_KEY].asBool()) {
                    continue;
                }

                // if indexing a primary key, the algorithm finishes,
                // since we've found the unique record with this key.
                static_hash_file.close();
                return located_records;
            }

            // seeks to the next overflow page
            seek_bucket = bucket.next_bucket;
        } while (seek_bucket != BUCKET_NULL_POINTER);

        static_hash_file.close();

        // finally, if any records was found, returns the vector and throws an error otherwise.
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

        long long bucket_to_insert = BUCKET_NULL_POINTER;
        long long seek_prev_bucket;

        do {
            static_hash_file.seekg(seek_bucket);
            bucket.read(static_hash_file);

            // if the current bucket is not full...
            if (bucket.num_records < bucket.bucket_capacity) {
                // and also there is not a candidate to insert,
                // assign the bucket to insert to the current bucket position.
                if (bucket_to_insert == BUCKET_NULL_POINTER) {
                    bucket_to_insert = seek_bucket;
                }

                // if not indexing a primary key, we're done,
                // since we've found a non-full bucket to insert the new record.
                if (!metadata_json[PRIMARY_KEY].asBool()) {
                    break;
                }
            }

            // only if indexing a primary key, we iterate through the bucket to check for a duplicate key
            // and throw an error if it is found.
            if (metadata_json[PRIMARY_KEY].asBool()) {
                for (int j = 0; j < bucket.num_records; ++j) {
                    if (!is_equal(get_indexed_field(record), get_indexed_field(bucket.records[j]))) {
                        continue;
                    }
                    static_hash_file.close();
                    throw std::runtime_error(REPEATED_KEY);
                }
            }

            seek_prev_bucket = seek_bucket;
            seek_bucket = bucket.next_bucket;

            // seeks to the next overflow page
        } while (seek_bucket != BUCKET_NULL_POINTER);

        // if no candidate for insertion is found:
        // - a new bucket is created and added to the end of the file.
        // - the previous bucket is read to reassign the "next_bucket" pointer
        //   to point to the beginning of the new bucket.
        if (bucket_to_insert  == BUCKET_NULL_POINTER) {
            Bucket<RecordType> new_bucket(metadata_json[BUCKET_CAPACITY].asInt());
            new_bucket.push_back(record);
            static_hash_file.seekp(0, std::ios::end);
            long long new_bucket_position = static_hash_file.tellg();
            new_bucket.write(static_hash_file);

            static_hash_file.seekg(seek_prev_bucket);
            bucket.read(static_hash_file);
            bucket.next_bucket = new_bucket_position;
            static_hash_file.seekp(seek_prev_bucket);
            bucket.write(static_hash_file);
        }
        // otherwise, it reads the non-full bucket,
        // adds the new record, and saves the changes to the file.
        else {
            static_hash_file.seekg(bucket_to_insert);
            bucket.read(static_hash_file);
            bucket.push_back(record);
            static_hash_file.seekp(bucket_to_insert);
            bucket.write(static_hash_file);
        }

        static_hash_file.close();
    }

    void remove(KeyType key) {
        int i = static_cast<int>(hash_function(key) % metadata_json[HASH_TABLE_SIZE].asInt());
        Bucket<RecordType> bucket(metadata_json[BUCKET_CAPACITY].asInt());

        long long seek_bucket = i * bucket.size_of();
        static_hash_file.open(metadata_json[INDEX_FULL_PATH].asString(), std::ios::in | std::ios::out | std::ios::binary);

        long long seek_prev_bucket = BUCKET_NULL_POINTER;
        bool any_found = false;

        do {
            static_hash_file.seekg(seek_bucket);
            bucket.read(static_hash_file);

            // iterates through the buckets looking for records with the key to remove
            for (int j = 0; j < bucket.num_records; ++j) {
                if (!is_equal(get_indexed_field(bucket.records[j]), key)) {
                    continue;
                }

                any_found = true;
                bucket.remove_at(j);

                // if, after removing a record, the current bucket becomes empty,
                // and there exists a previous bucket that points to this empty bucket,
                // we read the previous bucket to link the next overflow page to the empty bucket's next pointer.
                // This action "omits" the empty bucket in subsequent searches.
                if (bucket.is_empty() && seek_prev_bucket != BUCKET_NULL_POINTER) {
                    Bucket<RecordType> prev_bucket(metadata_json[BUCKET_CAPACITY].asInt());
                    static_hash_file.seekg(seek_prev_bucket);
                    prev_bucket.read(static_hash_file);
                    prev_bucket.next_bucket = bucket.next_bucket;
                    static_hash_file.seekp(seek_prev_bucket);
                    prev_bucket.write(static_hash_file);
                }

                // update the bucket information in the file
                static_hash_file.seekp(seek_bucket);
                bucket.write(static_hash_file);

                // if indexing a primary key, the algorithm finishes,
                // since we've successfully removed the unique record with this key.
                if (metadata_json[PRIMARY_KEY].asBool()) {
                    break;
                }
            }

            // seeks to the next overflow page
            seek_prev_bucket = seek_bucket;
            seek_bucket = bucket.next_bucket;

        } while (seek_bucket != BUCKET_NULL_POINTER);

        static_hash_file.close();

        if (any_found) {
            return;
        }
        throw std::runtime_error(KEY_NOT_FOUND);
    }
};

#endif //STATIC_HASH_STATIC_HASH_H
