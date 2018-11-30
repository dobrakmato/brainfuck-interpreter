cmake_minimum_required(VERSION 3.10)
project(brainfuck_interpreter)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(gtest)
include_directories(gtest/googletest/include)
include_directories(gtest/googlemock/include)

add_executable(brainfuck_interpreter src/Interpreter.h src/Interpreter.cpp src/main.cpp test/test_interpreter.cpp src/Hashmap.cpp src/Hashmap.h src/types.h)
target_link_libraries(brainfuck_interpreter gtest gtest_main)