#ifndef BRAINFUCK_INTERPRETER_JIT_H
#define BRAINFUCK_INTERPRETER_JIT_H

#include <string>
#include <chrono>
#include "Asm.h"
#include "Interpreter.h"
#include "Abi.h"

#define MAX_NESTING 256
#define MEMORY_SIZE 320000

using namespace std;
using namespace std::chrono;

class Jit {
private:
    string program;
    Asm compiled;
    Abi *abi;
    int8_t *jitMemory;
public:
    Jit() : compiled(Asm(4096 * 128)) {
        jitMemory = static_cast<int8_t *>(malloc(sizeof(int8_t) * MEMORY_SIZE));
        std::fill(jitMemory, jitMemory+MEMORY_SIZE, 0);

#ifdef PLATFORM_WINDOWS
        abi = new WindowsX64Abi();
#else
        abi = new LinuxX64Abi();
#endif
    }

    void loadProgram(const string &program) {
        this->program = program;
    }

    const char memoryAt(uint64_t address) {
        return jitMemory[address];
    }

    void compile();

    long run();

    void printStats();
};

#endif //BRAINFUCK_INTERPRETER_JIT_H
