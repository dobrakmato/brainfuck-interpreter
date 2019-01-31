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

#define CHAR(x) static_cast<int8_t>(x)

enum Register {
    RAX = 0,
    RCX = 1,
    RDX = 2,
    RBX = 3,
    RSP = 4,
    RBP = 5,
    RSI = 6,
    RDI = 7,
    R8 = 8,
    R9 = 9,
    R10 = 10,
    R11 = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15,
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
    MEMORY_R8 = 8,
    MEMORY_R9 = 9,
    MEMORY_R10 = 10,
    MEMORY_R11 = 11,
    MEMORY_R12 = 12,
    MEMORY_R13 = 13,
    MEMORY_R14 = 14,
    MEMORY_R15 = 15,
};

typedef int32_t(*fn)();

class Asm {
private:
    int8_t *m_dest;
    uint32_t m_addr = 0;
    uint32_t m_size;
    std::map<std::string, uint32_t> m_labels;

    const void imm(int8_t imm8);

    const void imm(int32_t imm32);

    const void imm(int64_t imm64);

public:
    explicit Asm(uint32_t size) : m_size(size) {
#ifndef PLATFORM_WINDOWS
        m_dest = static_cast<int8_t *>(mmap(nullptr, m_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
#else
        m_dest = static_cast<int8_t *>(VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
#endif
        /* fill with INT3 to easier debugging */
        std::fill(m_dest, m_dest + m_size, 0xCC);
    }

    const std::map<std::string, uint32_t> labels() {
        return m_labels;
    }

    const uint32_t address() {
        return m_addr;
    }

    const uint32_t allocated() {
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

    void ADD(Register reg, int32_t imm32);

    void ADD(Memory reg, int32_t imm32);

    void ADD(Memory reg, int8_t imm8);

    void SUB(Register reg, int32_t imm32);

    void SUB(Memory reg, int32_t imm32);

    void SUB(Memory reg, int8_t imm8);

    void XOR(Register reg);

    void MOV(Register reg, int32_t imm32);

    void MOV(Register reg, int64_t imm64);

    void MOV(Register write, Register read);

    void MOV(Register write, Memory read);

    void MOV(Memory write, Register read);

    void CMP(Register reg, int32_t imm32);

    void CMP(Memory reg, int32_t imm32);

    void CMP(Memory reg, int8_t imm8);

    void JNE(int32_t relativeAddress);

    void JE(int32_t relativeAddress);

    void JNE(const std::string &label);

    void JE(const std::string &label);

    void CALL(Register reg);

    void RET();

    int32_t run();
};


#endif //BRAINFUCK_INTERPRETER_ASM_H
