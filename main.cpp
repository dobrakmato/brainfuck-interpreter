#include <iostream>
#include <cstdio>

const char OP_INCREMENT_POINTER = '>';
const char OP_DECREMENT_POINTER = '<';
const char OP_INCREMENT_VALUE = '+';
const char OP_DECREMENT_VALUE = '-';
const char OP_PRINT = '.';
const char OP_LOAD = ',';
const char OP_JMP_FW = '[';
const char OP_JMP_BK = ']';

const int MEMORY_SIZE = 30000;

void interpret(std::string program) {

    char memory[MEMORY_SIZE];
    int pointer = 0;

    int pl = program.length();
    int pc = 0;

    // while not end of program
    while (pc != pl) {

        // fetch next op
        char op = program[pc];

        switch (op) {
            case OP_INCREMENT_POINTER:
                pointer++;
                pc++;
                break;
            case OP_DECREMENT_POINTER:
                pointer--;
                pc++;
                break;
            case OP_INCREMENT_VALUE:
                memory[pointer]++;
                pc++;
                break;
            case OP_DECREMENT_VALUE:
                memory[pointer]--;
                pc++;
                break;
            case OP_PRINT:
                putchar(memory[pointer]);
                pc++;
                break;
            case OP_LOAD:
                memory[pointer] = static_cast<char>(getchar());
                pc++;
                break;
            case OP_JMP_FW:
                if (memory[pointer] == 0) {
                    int paren = 1;
                    int pos = pc + 1;

                    while (true) {
                        if (program[pos] == OP_JMP_BK) {
                            paren--;

                            if (paren == 0) {
                                break;
                            }
                        }

                        if (program[pos] == OP_JMP_FW) {
                            paren++;
                        }

                        pos++;
                    }

                    pc = pos + 1;
                } else {
                    pc++;
                }
                break;
            case OP_JMP_BK:
                if (memory[pointer] != 0) {
                    int paren = 1;
                    int pos = pc - 1;

                    while (true) {

                        if (program[pos] == OP_JMP_FW) {
                            paren--;

                            if (paren == 0) {
                                break;
                            }
                        }

                        if (program[pos] == OP_JMP_BK) {
                            paren++;
                        }

                        pos--;
                    }

                    pc = pos;
                } else {
                    pc++;
                }
                break;
            default:
                throw std::runtime_error("Invalid instruction");
        }
    }
}




// adfadfadf(adfadf(afadf)f)fff
//                         ^




int main(int argc, char *argv[]) {
    interpret("++++[>+++++<-]>[<+++++>-]+<+[>[>+>+<<-]++>>[<<+>>-]>>>[-]++>[-]+>>>+[[-]++++++>>>]<<<[[<++++++++<++>>-]+<.<[>----<-]<]<<[>>>>>[>>>[-]+++++++++<[>-<-]+++++++++>[-[<->-]+[<<<]]<[>+<-]>]<<-]<<-]");
    return 0;
}