//
// Created by juan diego on 9/5/23.
//

#ifndef STATIC_HASH_PROPERTY_H
#define STATIC_HASH_PROPERTY_H

#include "dependencies.h"

using namespace json_keys;

struct Property {
    std::string directory_path;
    std::string index_file_name;
    std::string metadata_file_name;
    int hash_table_size;
    int bucket_capacity;
    bool primary_key;

    explicit Property(std::string directory_path,
                      std::string metadata_file_name,
                      std::string index_file_name,
                      int hash_table_size,
                      int bucket_capacity,
                      bool primary_key)
            :   directory_path(std::move(directory_path)),
                metadata_file_name(std::move(metadata_file_name)),
                index_file_name(std::move(index_file_name)),
                hash_table_size(hash_table_size),
                bucket_capacity(bucket_capacity),
                primary_key(primary_key){
    }

    [[nodiscard]] Json::Value json_value() const {
        Json::Value json_format;

        json_format[DIRECTORY_PATH] = directory_path;
        json_format[INDEX_FULL_PATH] = directory_path  + index_file_name;
        json_format[METADATA_FULL_PATH] = directory_path + metadata_file_name;
        json_format[HASH_TABLE_SIZE] = hash_table_size;
        json_format[BUCKET_CAPACITY] = bucket_capacity;
        json_format[PRIMARY_KEY] = primary_key;

        return json_format;
    }
};


#endif //STATIC_HASH_PROPERTY_H
