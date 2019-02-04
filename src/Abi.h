

#ifndef BRAINFUCK_INTERPRETER_ABI_H
#define BRAINFUCK_INTERPRETER_ABI_H

#include "Asm.h"

class Abi {
public:
    virtual Register firstParameterRegister() = 0;
};

class WindowsX64Abi : public Abi {
    Register firstParameterRegister() override;
};

class LinuxX64Abi : public Abi {
    Register firstParameterRegister() override;
};

#endif //BRAINFUCK_INTERPRETER_ABI_H
