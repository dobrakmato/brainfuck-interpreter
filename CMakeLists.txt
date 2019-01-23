cmake_minimum_required(VERSION 3.9)
project(brainfuck_interpreter)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(gtest)
include_directories(gtest/googletest/include)
include_directories(gtest/googlemock/include)

add_executable(brainfuck_interpreter src/Interpreter.h src/Interpreter.cpp src/main.cpp src/Hashmap.h src/types.h)
target_link_libraries(brainfuck_interpreter)

add_executable(unit_tests src/Interpreter.h src/Interpreter.cpp test/test_interpreter.cpp src/Hashmap.h src/types.h test/test_hashmap.cpp)
target_link_libraries(unit_tests gtest gtest_main)

enable_testing()
add_test(NAME unit COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests)