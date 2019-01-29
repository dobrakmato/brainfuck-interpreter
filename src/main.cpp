#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include "Interpreter.h"
#include "Asm.h"
#include "Jit.h"

using namespace std::chrono;

int main(int argc, char *argv[]) {

    auto jit = Jit();
    auto interpreter = Interpreter();

    if (argc < 2) {
        printf("Usage: brainfuck_interpreter SOURCE_FILE\n");
        exit(1);
    }

    auto fh = fopen(argv[1], "rb"); // we use b to be safe on windows systems

    if (!fh) {
        printf("File '%s' not found!", argv[1]);
        exit(-1);
    }

    fseek(fh, 0, SEEK_END);
    auto size = ftell(fh);

    fseek(fh, 0, SEEK_SET);
    char program[size];
    fread(program, 1, size, fh);
    fclose(fh);

    jit.loadProgram(program);
    jit.compile();

    auto jitStart = high_resolution_clock::now();
    jit.run();
    auto jitEnd = high_resolution_clock::now();

    interpreter.loadProgram(program);

    auto interpretStart = high_resolution_clock::now();
    interpreter.interpret();
    auto interpretEnd = high_resolution_clock::now();

    interpreter.printStats();
    jit.printStats();
    printf("STAT_INTERPRET_TIME=%f\n", duration_cast<duration<double>>(interpretEnd - interpretStart).count());
    printf("STAT_COMPILED_TIME=%f\n", duration_cast<duration<double>>(jitEnd - jitStart).count());

    return 0;
}