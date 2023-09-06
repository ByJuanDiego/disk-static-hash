//
// Created by juan diego on 9/5/23.
//

#ifndef STATIC_HASH_PROPERTY_H
#define STATIC_HASH_PROPERTY_H

#include "dependencies.h"

using namespace json_keys;

struct Property {
    std::string index_file_name;
    std::string metadata_file_name;
    int hash_table_size;
    int bucket_capacity;
    bool primary_key;

    explicit Property(std::string metadata_file_name,
                      std::string index_file_name,
                      int hash_table_size,
                      int bucket_capacity,
                      bool primary_key)
            :   metadata_file_name(std::move(metadata_file_name)),
                index_file_name(std::move(index_file_name)),
                hash_table_size(hash_table_size),
                bucket_capacity(bucket_capacity),
                primary_key(primary_key){
    }

    [[nodiscard]] Json::Value json_value() const {
        Json::Value json_format;

        json_format[INDEX_FILE_NAME] = index_file_name;
        json_format[HASH_TABLE_SIZE] = hash_table_size;
        json_format[BUCKET_CAPACITY] = bucket_capacity;
        json_format[METADATA_FILE_NAME] = metadata_file_name;
        json_format[PRIMARY_KEY] = primary_key;

        return json_format;
    }
};


#endif //STATIC_HASH_PROPERTY_H
