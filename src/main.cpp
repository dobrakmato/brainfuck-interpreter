#include <iostream>
#include <gtest/gtest.h>
#include "Interpreter.h"

int main(int argc, char *argv[]) {
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

    interpreter.loadProgram(program);
    interpreter.interpret();
    interpreter.printStats();
    return 0;
}