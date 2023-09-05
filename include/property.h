//
// Created by juan diego on 9/5/23.
//

#ifndef STATIC_HASH_PROPERTY_H
#define STATIC_HASH_PROPERTY_H

#include "dependencies.h"

struct Property {
    std::string index_file_name;
    std::string metadata_file_name;
    int hash_tabla_size;
    int bucket_capacity;

    explicit Property(std::string metadata_file_name,
                      std::string index_file_name,
                      int hash_table_size,
                      int bucket_capacity)
            :   metadata_file_name(std::move(metadata_file_name)),
                index_file_name(std::move(index_file_name)),
                hash_tabla_size(hash_table_size),
                bucket_capacity(bucket_capacity){
    }

    [[nodiscard]] Json::Value json_value() const {
        Json::Value json_format;

        json_format["index_file_name"] = index_file_name;
        json_format["metadata_file_name"] = metadata_file_name;
        json_format["hash_table_size"] = hash_tabla_size;
        json_format["bucket_capacity"] = bucket_capacity;

        return json_format;
    }
};


#endif //STATIC_HASH_PROPERTY_H
