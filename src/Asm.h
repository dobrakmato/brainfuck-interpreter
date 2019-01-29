#ifndef BRAINFUCK_INTERPRETER_ASM_H
#define BRAINFUCK_INTERPRETER_ASM_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define PLATFORM_WINDOWS
#endif

#include <stdio.h>
#include <cstdarg>

#ifndef PLATFORM_WINDOWS
#include <sys/mman.h>
#else

#include <memoryapi.h>

#endif

#include <map>
#include "types.h"

#define CHAR(x) static_cast<char>(x)

enum Register {
    RAX = 0,
    RCX = 1,
    RDX = 2,
    RBX = 3,
    RSP = 4,
    RBP = 5,
    RSI = 6,
    RDI = 7,
};

enum Memory {
    MEMORY_RAX = 0,
    MEMORY_RCX = 1,
    MEMORY_RDX = 2,
    MEMORY_RBX = 3,
    MEMORY_RSP = 4,
    MEMORY_RBP = 5,
    MEMORY_RSI = 6,
    MEMORY_RDI = 7,
};

typedef long(*fn)();

class Asm {
private:
    char *m_dest;
    uint32 m_addr = 0;
    uint32 m_size;
    std::map<std::string, uint32> m_labels;

    const void imm(char imm8);

    const void imm(long imm32);

public:
    explicit Asm(uint32 size) : m_size(size) {
#ifndef PLATFORM_WINDOWS
        m_dest = static_cast<char *>(mmap(nullptr, m_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
#else
        m_dest = static_cast<char *>(VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
#endif
    }

    const std::map<std::string, uint32> labels() {
        return m_labels;
    }

    const uint32 address() {
        return m_addr;
    }

    const uint32 allocated() {
        return m_size;
    }

    void label(std::string name) {
        m_labels.emplace(name, m_addr);
    }

    void setAddressByLabel(const std::string &name) {
        m_addr = m_labels.at(name);
    }

    void INC(Memory reg);

    void DEC(Memory reg);

    void INC(Register reg);

    void DEC(Register reg);

    void ADD(Register reg, long imm32);

    void ADD(Memory reg, long imm32);

    void ADD(Memory reg, char imm8);

    void SUB(Register reg, long imm32);

    void SUB(Memory reg, long imm32);

    void SUB(Memory reg, char imm8);

    void XOR(Register reg);

    void MOV(Register reg, long imm32);

    void MOV(Register write, Register read);

    void MOV(Register write, Memory read);

    void MOV(Memory write, Register read);

    void CMP(Register reg, long imm32);

    void CMP(Memory reg, long imm32);

    void CMP(Memory reg, char imm8);

    void JNE(long relativeAddress);

    void JE(long relativeAddress);

    void JNE(const std::string &label);

    void JE(const std::string &label);

    void CALL(Register reg);

    void RET();

    int32 run();
};


#endif //BRAINFUCK_INTERPRETER_ASM_H
