#include "Asm.h"

const void Asm::imm(char imm8) {
    m_dest[m_addr++] = imm8;
}

const void Asm::imm(long imm32) {
    m_dest[m_addr++] = CHAR(imm32 & 0xFF);
    m_dest[m_addr++] = CHAR((imm32 >> 8) & 0xFF);
    m_dest[m_addr++] = CHAR((imm32 >> 16) & 0xFF);
    m_dest[m_addr++] = CHAR((imm32 >> 24) & 0xFF);
}


void Asm::INC(Memory reg) {
    m_dest[m_addr++] = CHAR(0xFE); // inc
    m_dest[m_addr++] = CHAR(0x00 + reg); // opcode 0 + [reg]
}

void Asm::DEC(Memory reg) {
    m_dest[m_addr++] = CHAR(0xFE); // dec
    m_dest[m_addr++] = CHAR(0x08 + reg); // opcode 1 + [reg]
}

void Asm::INC(Register reg) {
    m_dest[m_addr++] = CHAR(0x48); // REX prefix
    m_dest[m_addr++] = CHAR(0xFE); // inc
    m_dest[m_addr++] = CHAR(0xc0 + reg); // opcode 0 + reg
}

void Asm::DEC(Register reg) {
    m_dest[m_addr++] = CHAR(0x48); // REX prefix
    m_dest[m_addr++] = CHAR(0xFE); // dec
    m_dest[m_addr++] = CHAR(0xc8 + reg); // opcode 1 + reg
}

// -------

void Asm::ADD(Register reg, int32 imm32) {
    m_dest[m_addr++] = CHAR(0x81); // add
    m_dest[m_addr++] = CHAR(0xc0 + reg); // rax

    imm(imm32);
}

void Asm::ADD(Memory reg, int32 imm32) {
    m_dest[m_addr++] = CHAR(0x81); // add
    m_dest[m_addr++] = CHAR(0x00 + reg); // rax

    imm(imm32);
}

void Asm::ADD(Memory reg, int8 imm8) {
    m_dest[m_addr++] = CHAR(0x80); // add
    m_dest[m_addr++] = CHAR(0x00 + reg); // reg

    imm(imm8);
}

void Asm::SUB(Register reg, int32 imm32) {
    m_dest[m_addr++] = CHAR(0x81); // sub
    m_dest[m_addr++] = CHAR(0xe8 + reg); // opcode 5 + reg

    imm(imm32);
}

void Asm::SUB(Memory reg, int32 imm32) {
    m_dest[m_addr++] = CHAR(0x81); // sub
    m_dest[m_addr++] = CHAR(0x38 + reg); // opcode 5 + reg

    imm(imm32);
}

void Asm::SUB(Memory reg, int8 imm8) {
    m_dest[m_addr++] = CHAR(0x80); // sub
    m_dest[m_addr++] = CHAR(0x28 + reg); // reg

    imm(imm8);
}

void Asm::XOR(Register reg) {
    auto val = (reg << 3) | reg;

    m_dest[m_addr++] = CHAR(0x31); // xor
    m_dest[m_addr++] = CHAR(0xc0 + val); // reg, reg
}

void Asm::MOV(Register reg, int32 imm32) {
    m_dest[m_addr++] = CHAR(0xC7); // mov
    m_dest[m_addr++] = CHAR(0xc0 + reg); // opcode 0 + reg

    imm(imm32);
}

void Asm::MOV(Register write, Register read) {
    m_dest[m_addr++] = CHAR(0x89);         // mov
    m_dest[m_addr++] = CHAR(0xc0 + write); // write,
    m_dest[m_addr++] = CHAR(0xc0 + read);  // read
}

void Asm::MOV(Register write, Memory read) {
    m_dest[m_addr++] = CHAR(0x8a);         // mov
    m_dest[m_addr++] = CHAR(0x00 + write * 8 + read); // [write], read
}

void Asm::MOV(Memory write, Register read) {
    m_dest[m_addr++] = CHAR(0x88);         // mov
    m_dest[m_addr++] = CHAR(0x00 + 8 * read + write); // [read], write
}

void Asm::CMP(Register reg, int32 imm32) {
    m_dest[m_addr++] = CHAR(0x81); // cmp
    m_dest[m_addr++] = CHAR(0xf8 + reg); // opcode 7 + register

    imm(imm32);
}

void Asm::CMP(Memory reg, int32 imm32) {
    m_dest[m_addr++] = CHAR(0x81); // cmp
    m_dest[m_addr++] = CHAR(0x38 + reg); // opcode 7 + [register]

    imm(imm32);
}

void Asm::CMP(Memory reg, int8 imm8) {
    m_dest[m_addr++] = CHAR(0x80); // cmp
    m_dest[m_addr++] = CHAR(0x38 + reg); // opcode 7 + [register]

    imm(imm8);
}

void Asm::JNE(int32 relativeAddress) {
    m_dest[m_addr++] = CHAR(0x0f); // 0F prefix
    m_dest[m_addr++] = CHAR(0x85); // jne

    imm(relativeAddress);
}


void Asm::JE(int32 relativeAddress) {
    m_dest[m_addr++] = CHAR(0x0f); // 0F prefix
    m_dest[m_addr++] = CHAR(0x84); // je

    imm(relativeAddress);
}

void Asm::JNE(const std::string &label) {
    auto labelAddress = m_labels.at(label);
    auto relative = labelAddress - (m_addr + 6); // +6 (length of JNE instruction)
    JNE(relative);
}

void Asm::JE(const std::string &label) {
    auto labelAddress = m_labels.at(label);
    auto relative = labelAddress - (m_addr + 6); // +6 (length of JE instruction)
    JE(relative);
}

void Asm::CALL(Register reg) {
    SUB(RSP, 40);
    m_dest[m_addr++] = CHAR(0xff); // call
    m_dest[m_addr++] = CHAR(0xd0 + reg); // opcode 2 + reg
    ADD(RSP, 40);
}

void Asm::RET() {
    m_dest[m_addr++] = CHAR(0xc3); // ret
}

int32 Asm::run() {
#ifdef PLATFORM_WINDOWS
    DWORD old;
    auto s = sizeof(*m_dest) * m_size;
    VirtualProtect(m_dest, s, PAGE_EXECUTE_READ, &old);
#else
    mprotect(m_dest, m_size, PROT_EXEC | PROT_READ);
#endif

    auto f = reinterpret_cast<fn>(m_dest);
    return f();
}