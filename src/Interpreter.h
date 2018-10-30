//
// Created by Matej on 30.10.2018.
//

#ifndef BRAINFUCK_INTERPRETER_INTERPRETER_H
#define BRAINFUCK_INTERPRETER_INTERPRETER_H

#include <string>

typedef unsigned char uint8;
typedef unsigned long long uint32;

const uint8 OP_INCREMENT_POINTER = '>';
const uint8 OP_DECREMENT_POINTER = '<';
const uint8 OP_INCREMENT_VALUE = '+';
const uint8 OP_DECREMENT_VALUE = '-';
const uint8 OP_PRINT = '.';
const uint8 OP_LOAD = ',';
const uint8 OP_JMP_FW = '[';
const uint8 OP_JMP_BK = ']';

const int MEMORY_SIZE = 30000;

/**
 * Simple but powerful Brainfuck interpreter.
 */
class Interpreter {
private:
    uint8 memory[MEMORY_SIZE] = {0};
    uint32 pointer = 0;
    uint32 programCounter = 0;
    std::string program;
    std::string outputBuffer;
    bool printToStdOut = true;

    uint8 fetchOp();

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

    uint8 memoryAt(int address);

    void loadProgram(const std::string &program);

    std::string interpret();
};

#endif //BRAINFUCK_INTERPRETER_INTERPRETER_H
