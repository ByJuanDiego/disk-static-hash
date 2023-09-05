//
// Created by juan diego on 9/4/23.
//

#ifndef STATIC_HASH_STATIC_HASH_H
#define STATIC_HASH_STATIC_HASH_H

#include "bucket.h"
#include "property.h"


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
    Index index_function;
    Equal equal_function;

    void load_metadata() {
        metadata_file >> metadata_json;
    }

    void create_empty_static_hash(const Property& property) {
        metadata_file.open(property.metadata_file_name, std::ios::out);
        metadata_json = property.json_value();
        metadata_file << metadata_json;
        metadata_file.close();
        
        static_hash_file.open(metadata_json[INDEX_FILE_NAME].asString(), std::ios::out);
        for (int i = 0; i < metadata_json[HASH_TABLE_SIZE].asInt(); ++i) {
            Bucket<RecordType> bucket(metadata_json[BUCKET_CAPACITY].asInt());
            bucket.write(static_hash_file);
        }
        static_hash_file.close();
    }

public:

    explicit StaticHash(const Property& property, Index index, Hash hash = Hash(), Equal equal = Equal())
    :   index_function(index),
        hash_function(hash),
        equal_function(equal) {
        metadata_file.open(property.metadata_file_name, std::ios::in);
        if (metadata_file.good()) {
            load_metadata();
        } else {
            metadata_file.close();
            create_empty_static_hash(property);
        }
        metadata_file.close();
    }

    RecordType search(KeyType key) {
        int i = hash_function(key) % metadata_json[HASH_TABLE_SIZE].asInt();
        Bucket<RecordType> bucket(metadata_json[BUCKET_CAPACITY]);

        long long bucket_position = i * bucket.size_of();

        static_hash_file.open(metadata_json[INDEX_FILE_NAME].asString(), std::ios::in | std::ios::binary);
        static_hash_file.seekg(bucket_position);
        bucket.read(static_hash_file);
    }

    void insert(const RecordType& record) {

    }

    void remove(KeyType key) {

    }
};

#endif //STATIC_HASH_STATIC_HASH_H
