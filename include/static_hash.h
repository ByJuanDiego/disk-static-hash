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
    typename Hash = std::hash<KeyType>,
    typename Index = std::function<KeyType(RecordType)>,
    typename Equal = std::equal_to<KeyType>
>
struct StaticHash {
private:

    std::fstream metadata_file;
    std::fstream static_hash_file;

    Json::Value metadata_json;

    Hash hash_function;
    Index index_function;

    void load_metadata() {
        metadata_file >> metadata_json;
    }

    void create_empty_static_hash(const Property& property) {
        metadata_file.open(property.metadata_file_name, std::ios::out);
        metadata_json = property.json_value();
        metadata_file << metadata_json;
        metadata_file.close();
        
        static_hash_file.open(metadata_json["index_file_name"].asString(), std::ios::out);
        for (int i = 0; i < metadata_json["hash_table_size"].asInt(); ++i) {
            Bucket<RecordType> bucket(metadata_json["bucket_capacity"].asInt());
            bucket.write(static_hash_file);
        }
        static_hash_file.close();
    }

public:

    explicit StaticHash(const Property& property) {
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

    }

    void insert(const RecordType& record) {

    }

    void remove(KeyType key) {

    }
};

#endif //STATIC_HASH_STATIC_HASH_H
