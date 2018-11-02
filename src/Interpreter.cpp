
#include <stdexcept>
#include "Interpreter.h"

void Interpreter::setPrintToStdout(bool printToStdOut0) {
    printToStdOut = printToStdOut0;
}

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

void Interpreter::opPrint() {
    boundsCheck();
    if (printToStdOut) {
        putchar(memory[pointer]);
    }
    outputBuffer += memory[pointer];
    programCounter++;
}

void Interpreter::opLoad() {
    boundsCheck();
    memory[pointer] = static_cast<uint8>(getchar());
    programCounter++;
}

void Interpreter::opJmpFwd() {
    if (memory[pointer] == 0) {
        uint32 paren = 1;
        uint32 pos = programCounter + 1;

        while (true) {
            if (program[pos] == OP_JMP_BK) {
                if (jumpTable[pos] != JUMP_TABLE_INITIAL_VALUE) {
                    pos = jumpTable[pos];
                    break;
                }
                paren--;

                if (paren == 0) {
                    jumpTable[programCounter + 1] = pos;
                    break;
                }
            }

            if (program[pos] == OP_JMP_FW) {
                paren++;
            }

            pos++;

            if (pos >= program.length()) {
                printf("Unmatched brackets.\n");
                exit(4);
            }
        }

        programCounter = pos + 1;
    } else {
        programCounter++;
    }
}

void Interpreter::opJmpBk() {
    if (memory[pointer] != 0) {
        uint32 paren = 1;
        uint32 pos = programCounter - 1;

        while (true) {

            if (program[pos] == OP_JMP_FW) {
                if (jumpTable[pos] != JUMP_TABLE_INITIAL_VALUE) {
                    pos = jumpTable[pos];
                    break;
                }

                paren--;

                if (paren == 0) {
                    jumpTable[programCounter - 1] = pos;
                    break;
                }
            }

            if (program[pos] == OP_JMP_BK) {
                paren++;
            }

            pos--;

            if (pos < 0) {
                printf("Unmatched brackets.\n");
                exit(4);
            }
        }

        programCounter = pos;
    } else {
        programCounter++;
    }
}

/**
 * Returns the next operation of program.
 * @return next operation
 */
uint8 Interpreter::fetchOp() {
    return static_cast<uint8>(program[programCounter]);
}

/***
 * Returns memory value at specified address.
 * @param address address to return value of
 * @return value of address
 */
uint8 Interpreter::memoryAt(int address) {
    return memory[address];
}

/**
 * Loads specified program into interpreter.
 * @param program0 program data
 */
void Interpreter::loadProgram(const std::string &program0) {
    program = program0;
}

/**
 * Interpretes the program previously loaded
 * via loadProgram() method and returns the
 * data it printed to stdout.
 * @return result of program
 */
std::string Interpreter::interpret() {
    std::fill_n(jumpTable, JUMP_TABLE_SIZE, JUMP_TABLE_INITIAL_VALUE);
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
            case OP_PRINT:
                opPrint();
                break;
            case OP_LOAD:
                opLoad();
                break;
            case OP_JMP_FW:
                opJmpFwd();
                break;
            case OP_JMP_BK:
                opJmpBk();
                break;
            default:
                // This is comment, ignore it.
                programCounter++;
                break;
        }
    }

    return outputBuffer;
}