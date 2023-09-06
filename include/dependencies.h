//
// Created by juan diego on 9/4/23.
//

#ifndef STATIC_HASH_DEPENDENCIES_H
#define STATIC_HASH_DEPENDENCIES_H


// Third party libraries
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <openssl/sha.h>

#include <json/json.h>


// STL libraries
#include <functional>
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>

// Own headers
#include "buffer_size.h"


namespace json_keys {
    std::string BUCKET_CAPACITY = "bucket_capacity";
    std::string HASH_TABLE_SIZE = "hash_table_size";
    std::string INDEX_FILE_NAME = "index_file_name";
    std::string METADATA_FILE_NAME = "metadata_file_name";
    std::string PRIMARY_KEY = "primary_key";
}

namespace error_logs {
    std::string KEY_NOT_FOUND = "Key not found";
    std::string REPEATED_KEY = "Repeated key";
}

namespace types {
    using uint256 = boost::multiprecision::uint256_t;
    using uint32 = uint32_t;
    using int64 = int64_t;
}

namespace constants {
    const unsigned long buffer_size = get_buffer_size();

    template <typename T>
    const int size_of = sizeof (T);
}

#endif //STATIC_HASH_DEPENDENCIES_H
