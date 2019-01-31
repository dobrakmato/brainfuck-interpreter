#ifndef BRAINFUCK_INTERPRETER_INTERPRETER_H
#define BRAINFUCK_INTERPRETER_INTERPRETER_H

#include <string>
#include <map>
#include "Hashmap.h"

const uint8_t OP_INCREMENT_POINTER = '>';
const uint8_t OP_DECREMENT_POINTER = '<';
const uint8_t OP_INCREMENT_VALUE = '+';
const uint8_t OP_DECREMENT_VALUE = '-';
const uint8_t OP_PRINT = '.';
const uint8_t OP_LOAD = ',';
const uint8_t OP_JMP_FW = '[';
const uint8_t OP_JMP_BK = ']';

const uint32_t MEMORY_SIZE = 30000;

/**
 * Simple but powerful Brainfuck interpreter.
 */
class Interpreter {
private:
    uint8_t memory[MEMORY_SIZE] = {0};
    uint32_t pointer = 0;
    uint32_t programCounter = 0;
    std::string program;
    std::string input;
    std::string outputBuffer;
    Hashmap<uint32_t, uint32_t>* jumpTable = new Hashmap<int unsigned, int unsigned>(256); // this for some reason does not compile with new Hashmap<uint32, uint32>()
    bool printToStdOut = true;

    uint8_t fetchOp();

    void boundsCheck() {
        if (pointer < 0 || pointer >= MEMORY_SIZE) {
            printf("Illegal memory acess detected!\n");
            exit(3);
        }
    }

    void opIncrementPointer();

    void opDecrementPointer();

    void opIncrementValue();

    void opDecrementValue();

    void opPrint();

    void opLoad();

    void opJmpFwd();

    void opJmpBk();

public:
    void setPrintToStdout(bool printToStdOut);

    uint8_t memoryAt(int address);

    void loadProgram(const std::string &program);

    std::string interpret();

    void printStats();

    void setInput(const std::string &input);
};

#endif //BRAINFUCK_INTERPRETER_INTERPRETER_H
