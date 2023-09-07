//
// Created by juan diego on 9/5/23.
//

#ifndef INDEX_HASH_FUNCTION_H
#define INDEX_HASH_FUNCTION_H


#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <openssl/sha.h>

/// Group of functions and callbacks related to hashing
namespace sha2 {

    /// Transform a hexadecimal to a 256 bits integer
    boost::multiprecision::uint256_t to_uint256(const std::string &hex) {
        boost::cnv::cstream converter;
        converter(std::hex)(std::skipws);
        auto f = apply<boost::multiprecision::uint256_t>(std::ref(converter));
        return f(hex);
    }

    /// Transform a string to his correspondent sha256 hash code
    std::string get_sha256(const std::string &str) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((const unsigned char *)str.c_str(), str.length(), hash);
        std::stringstream ss;
        for (unsigned char i: hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (unsigned) i;
        }
        return ss.str();
    }

    std::string get_sha256(const char *str) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((const unsigned char*) str, strlen(str), hash);
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
        boost::multiprecision::uint256_t operator()(const K &key) {
            return sha2::to_uint256(sha2::get_sha256(std::to_string(key)));
        }
    };

    /// sha256<K> specialization for strings
    template<>
    struct sha256<std::string> {
        boost::multiprecision::uint256_t operator()(const std::string &key) {
            return sha2::to_uint256(sha2::get_sha256(key));
        }
    };

    template <>
    struct sha256<const char *> {
        boost::multiprecision::uint256_t operator()(const char *key) {
            return sha2::to_uint256(sha2::get_sha256(key));
        }
    };
}


#endif //INDEX_HASH_FUNCTION_H
