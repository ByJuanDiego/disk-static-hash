cmake_minimum_required(VERSION 3.26)
project(index)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

include_directories(./utils ./include)

add_executable(index
        src/main.cpp
        utils/buffer_size.h
        include/static_hash.h
        include/dependencies.h
        include/bucket.h
        include/property.h
        utils/record.h
)

target_link_libraries(${PROJECT_NAME} ${JSONCPP_LIBRARIES})
