//
// Created by juan diego on 9/6/23.
//

#ifndef STATIC_HASH_FILE_UTILS_H
#define STATIC_HASH_FILE_UTILS_H

#include <string>
#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>

bool directory_exists(const std::string& path) {
    return std::filesystem::is_directory(path);
}

bool create_directory(const std::string& path) {
    int status = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if (status == 0) {
        return true;
    }
    return false;
}

#endif //STATIC_HASH_FILE_UTILS_H
