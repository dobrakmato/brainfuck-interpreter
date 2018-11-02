
#include <stdexcept>
#include "Interpreter.h"

void Interpreter::opIncrementPointer() {
    pointer++;
    programCounter++;
}

void Interpreter::opDecrementPointer() {
    pointer--;
    programCounter++;
}

void Interpreter::opIncrementValue() {
    boundsCheck();
    memory[pointer]++;
    programCounter++;
}

void Interpreter::opDecrementValue() {
    boundsCheck();
    memory[pointer]--;
    programCounter++;
}


uint8 Interpreter::fetchOp() {
    return static_cast<uint8>(program[programCounter]);
}

uint8 Interpreter::memoryAt(int address) {
    return memory[address];
}

void Interpreter::loadProgram(const std::string &program0) {
    program = program0;
}

std::string Interpreter::interpret() {
    uint32 programLength = program.length();
    programCounter = 0;

    while (programCounter != programLength) {
        auto op = fetchOp();

        switch (op) {
            case OP_INCREMENT_POINTER:
                opIncrementPointer();
                break;
            case OP_DECREMENT_POINTER:
                opDecrementPointer();
                break;
            case OP_INCREMENT_VALUE:
                opIncrementValue();
                break;
            case OP_DECREMENT_VALUE:
                opDecrementValue();
                break;
            default:
                // This is comment, ignore it.
                programCounter++;
                break;
        }
    }

    return outputBuffer;
}