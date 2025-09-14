cmake_minimum_required(VERSION 3.5.0)
project(labwork1 VERSION 0.1.0 LANGUAGES C CXX)

SET(CMAKE_CXX_STANDARD 23)

add_executable(labwork3 
    bin/main.cpp 
    lib/support_functions.h
    lib/support_functions.cpp
    lib/parser.h
    lib/game.h
    lib/files_worker.h)