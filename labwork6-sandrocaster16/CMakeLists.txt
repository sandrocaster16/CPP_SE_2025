cmake_minimum_required(VERSION 3.12)

project(
    labwork6
    VERSION 1.0
    DESCRIPTION "C++ Year 2025 labwork_6"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(cpr)
add_subdirectory(json)

add_subdirectory(lib)
add_subdirectory(bin)