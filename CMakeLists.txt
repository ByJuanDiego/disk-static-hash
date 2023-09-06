cmake_minimum_required(VERSION 3.26)
project(static_hash)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

include_directories(./utils ./include)

add_executable(${PROJECT_NAME}
        src/main.cpp
        utils/buffer_size.h
        include/static_hash.h
        include/dependencies.h
        include/bucket.h
        include/property.h
        utils/record.h
        utils/hash_function.h
        utils/file_utils.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${JSONCPP_LIBRARIES} ssl crypto)
