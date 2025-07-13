cmake_minimum_required(VERSION 3.13)

project(codecrafters-http-server)

file(GLOB_RECURSE SOURCE_FILES src/*.c)

set(CMAKE_C_STANDARD 23) # Enable the C23 standard

add_executable(http-server ${SOURCE_FILES})

find_package(ZLIB REQUIRED)

target_link_libraries(http-server ZLIB::ZLIB)

target_include_directories(http-server PRIVATE include)
