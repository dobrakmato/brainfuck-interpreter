cmake_minimum_required(VERSION 3.9)
project(brainfuck_interpreter)

set(CMAKE_CXX_STANDARD 11)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

set(MAKE_GENERATOR_PLATFORM x64)
set(CMAKE_C_FLAGS -m64)
set(CMAKE_CXX_FLAGS -m64)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif ()

add_subdirectory(gtest)
include_directories(gtest/googletest/include)
include_directories(gtest/googlemock/include)

file(COPY sample-programs DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

add_executable(brainfuck_interpreter src/Interpreter.h src/Interpreter.cpp src/main.cpp src/Hashmap.h src/Asm.cpp src/Asm.h src/Jit.cpp src/Jit.h src/Abi.cpp src/Abi.h)
target_link_libraries(brainfuck_interpreter)

add_executable(unit_tests src/Interpreter.h src/Interpreter.cpp test/test_interpreter.cpp src/Hashmap.h test/test_hashmap.cpp src/Jit.h src/Jit.cpp src/Asm.h src/Asm.cpp test/test_jit.cpp src/Abi.h src/Abi.cpp)
target_link_libraries(unit_tests gtest gtest_main)

enable_testing()
add_test(NAME unit COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})