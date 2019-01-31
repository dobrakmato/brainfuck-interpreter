#include "Jit.h"

double STAT_COMPILE_TIME = 0;
uint64_t STAT_ALLOCATED_BYTES = 0;
uint64_t STAT_COMPILED_BYTES = 0;

void Jit::printStats() {
    printf("STAT_COMPILED_BYTES=%lu\n", STAT_COMPILED_BYTES);
    printf("STAT_ALLOCATED_BYTES=%lu\n", STAT_ALLOCATED_BYTES);
    printf("STAT_COMPILE_TIME=%f\n", STAT_COMPILE_TIME);
}

long Jit::run() {
    return compiled.run();
}

void Jit::compile() {
    auto timeStart = high_resolution_clock::now();

    // rbx -> brainfuck pointer
    // rsi -> putchar fn
    // rdi -> getchar fn

    uint32_t parenDepth = 0;
    uint32_t parenIdStack[MAX_NESTING];
    auto jitMemoryAddr = reinterpret_cast<int64_t>(jitMemory);

    compiled.MOV(RBX, jitMemoryAddr);
    compiled.MOV(RSI, reinterpret_cast<int64_t>(putchar));
    compiled.MOV(RDI, reinterpret_cast<int64_t>(getchar));

    // step 1: compile bf to asm with invalid addresses in [ jumps
    for (auto ch : program) {
        switch (ch) {
            case OP_INCREMENT_POINTER:
                compiled.ADD(RBX, 1);
                break;
            case OP_DECREMENT_POINTER:
                compiled.SUB(RBX, 1);
                break;
            case OP_INCREMENT_VALUE:
                compiled.ADD(MEMORY_RBX, (int8_t) 1);
                break;
            case OP_DECREMENT_VALUE:
                compiled.SUB(MEMORY_RBX, (int8_t) 1);
                break;
            case OP_PRINT:
                compiled.MOV(RCX, MEMORY_RBX); // mov rcx, [rbx]
                compiled.CALL(RSI); // call putchar
                break;
            case OP_LOAD:
                compiled.CALL(RDI); // call getchar
                compiled.MOV(MEMORY_RBX, RAX); // mov [rbx], rax
                break;
            case OP_JMP_FW:
                parenDepth++;
                parenIdStack[parenDepth]++;

                compiled.label("[" + to_string(parenDepth) + "_" + to_string(parenIdStack[parenDepth]));
                compiled.CMP(MEMORY_RBX, (int8_t) 0);
                compiled.JE(0xDEADBEEF); // jump after matching ...]_....
                break;
            case OP_JMP_BK:
                compiled.CMP(MEMORY_RBX, (int8_t) 0);
                compiled.JNE("[" + to_string(parenDepth) + "_" +
                             to_string(parenIdStack[parenDepth])); // jump before matching ...._[....

                compiled.label("]" + to_string(parenDepth) + "_" + to_string(parenIdStack[parenDepth]));
                parenDepth--;
                break;
            default:
                // This is comment, ignore it.
                break;
        }
    }

    compiled.RET(); /* dont forget to stop program */

    STAT_ALLOCATED_BYTES = compiled.allocated();
    STAT_COMPILED_BYTES = compiled.address();

    // step 2: fix invalid addresses in [ jumps
    for (auto const &x: compiled.labels()) {

        // only fix [ jumps
        if (x.first[0] == '[') {
            compiled.setAddressByLabel(x.first); // reset internal codegen pointer

            compiled.CMP(MEMORY_RBX, (int8_t) 0);
            compiled.JE("]" + x.first.substr(1));
        }
    }

    if (parenDepth != 0) {
        throw "Unbalanced brackets!";
    }

    auto timeEnd = high_resolution_clock::now();
    STAT_COMPILE_TIME = duration_cast<duration<double>>(timeEnd - timeStart).count();
}
