#include <iostream>
#include <gtest/gtest.h>
#include "Interpreter.h"

int main(int argc, char *argv[]) {
    auto interpreter = Interpreter();

    if (argc != 2) {
        printf("Usage: brainfuck_interpreter SOURCE_FILE\n");
        exit(1);
    }

    if (strcmp(argv[1], "--tests") == 0) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }

    auto fh = fopen(argv[1], "rb"); // we use b to be safe on windows systems
    fseek(fh, 0, SEEK_END);
    auto size = ftell(fh);
    fseek(fh, 0, SEEK_SET);
    char program[size];
    fread(program, 1, size, fh);
    fclose(fh);

    interpreter.loadProgram(program);
    interpreter.interpret();
    return 0;
}