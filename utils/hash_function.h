//
// Created by juan diego on 9/5/23.
//

#ifndef INDEX_HASH_FUNCTION_H
#define INDEX_HASH_FUNCTION_H


#include "dependencies.h"


/// Group of functions and callbacks related to hashing
namespace sha2 {

    /// Transform a hexadecimal to a 256 bits integer
    types::uint256 to_uint256(const std::string &hex) {
        boost::cnv::cstream converter;
        converter(std::hex)(std::skipws);
        auto f = apply<types::uint256>(std::ref(converter));
        return f(hex);
    }

    /// Transform a string to his correspondent sha256 hash code
    std::string get_sha256(const std::string &str) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.c_str(), str.size());
        SHA256_Final(hash, &sha256);
        std::stringstream ss;
        for (unsigned char i: hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (unsigned) i;
        }
        return ss.str();
    }

    /**
     * Callback struct used to get sha256 from a key.
     *
     *
     * @tparam K the type of the key to be hashed
     */
    template<typename K>
    struct sha256 {
        types::uint256 operator()(const K &key) {
            return sha2::to_uint256(sha2::get_sha256(std::to_string(key)));
        }
    };

    /// sha256<K> specialization for strings
    template<>
    struct sha256<std::string> {
        types::uint256 operator()(const std::string &key) {
            return sha2::to_uint256(sha2::get_sha256(key));
        }
    };
}


#endif //INDEX_HASH_FUNCTION_H
