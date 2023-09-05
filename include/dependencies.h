//
// Created by juan diego on 9/4/23.
//

#ifndef STATIC_HASH_DEPENDENCIES_H
#define STATIC_HASH_DEPENDENCIES_H


// Third party libraries
#include <boost/multiprecision/cpp_int.hpp>
#include <json/json.h>


// STL libraries
#include <functional>
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>

// Own headers
#include "buffer_size.h"


namespace types {
    using uint256 = boost::multiprecision::uint256_t;
    using uint32 = uint32_t;
}

namespace constants {
    const unsigned long buffer_size = get_buffer_size();

    template <typename T>
    const int size_of = sizeof (T);
}

#endif //STATIC_HASH_DEPENDENCIES_H
