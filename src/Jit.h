//
// Created by Matej on 26.1.2019.
//

#ifndef BRAINFUCK_INTERPRETER_JIT_H
#define BRAINFUCK_INTERPRETER_JIT_H

#include <string>
#include <chrono>
#include "Asm.h"
#include "Interpreter.h"

#define MAX_NESTING 256

using namespace std;
using namespace std::chrono;

class Jit {
private:
    string program;
    Asm compiled;
    char jitMemory[32000] = {0};
public:
    Jit() : compiled(Asm(4096 * 128)) {
    }

    void loadProgram(const string &program) {
        this->program = program;
    }

    const char memoryAt(uint32 address) {
        return jitMemory[address];
    }

    void compile();

    long run();

    void printStats();
};

#endif //BRAINFUCK_INTERPRETER_JIT_H
