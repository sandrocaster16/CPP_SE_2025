cmake_minimum_required(VERSION 3.12)

project(
    ulist
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

# include_directories(lib)

add_subdirectory(bin)
add_subdirectory(lib)

add_executable(unrolled_list bin/main.cpp
        lib/unrolled_list.h)

target_include_directories(unrolled_list PUBLIC ${PROJECT_SOURCE_DIR}/lib)


enable_testing()
add_subdirectory(tests)
