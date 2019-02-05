#include "Asm.h"

const void Asm::imm(int8_t imm8) {
    m_dest[m_addr++] = imm8;
}

const void Asm::imm(int32_t imm32) {
    m_dest[m_addr++] = CHAR(imm32 & 0xFF);
    m_dest[m_addr++] = CHAR((imm32 >> 8) & 0xFF);
    m_dest[m_addr++] = CHAR((imm32 >> 16) & 0xFF);
    m_dest[m_addr++] = CHAR((imm32 >> 24) & 0xFF);
}

const void Asm::imm(int64_t imm64) {
    m_dest[m_addr++] = CHAR(imm64 & 0xFF);
    m_dest[m_addr++] = CHAR((imm64 >> 8) & 0xFF);
    m_dest[m_addr++] = CHAR((imm64 >> 16) & 0xFF);
    m_dest[m_addr++] = CHAR((imm64 >> 24) & 0xFF);
    m_dest[m_addr++] = CHAR((imm64 >> 32) & 0xFF);
    m_dest[m_addr++] = CHAR((imm64 >> 40) & 0xFF);
    m_dest[m_addr++] = CHAR((imm64 >> 48) & 0xFF);
    m_dest[m_addr++] = CHAR((imm64 >> 56) & 0xFF);
}

// =====================================================

void Asm::ADD(Register reg, int32_t imm32) {
    m_dest[m_addr++] = CHAR(REX_W | (reg >= R8 ? REX_B : 0)); // REX.W + REX.B?
    m_dest[m_addr++] = CHAR(0x81); // add
    m_dest[m_addr++] = CHAR(0xc0 + reg); // rax

    imm(imm32);
}

void Asm::ADD(Memory reg, int8_t imm8) {
    m_dest[m_addr++] = CHAR(0x80); // add
    m_dest[m_addr++] = CHAR(0x00 + reg); // reg

    imm(imm8);
}

void Asm::SUB(Register reg, int32_t imm32) {
    m_dest[m_addr++] = CHAR(REX_W | (reg >= R8 ? REX_B : 0));
    m_dest[m_addr++] = CHAR(0x81); // sub
    m_dest[m_addr++] = CHAR(0xe8 + reg); // opcode 5 + reg

    imm(imm32);
}

void Asm::SUB(Memory reg, int8_t imm8) {
    m_dest[m_addr++] = CHAR(0x80); // sub
    m_dest[m_addr++] = CHAR(0x28 + reg); // reg

    imm(imm8);
}

void Asm::MOV(Register reg, int64_t imm64) {
    m_dest[m_addr++] = CHAR(0x48 | (reg >= R8 ? REX_B : 0)); // REX.W
    m_dest[m_addr++] = CHAR(0xB8 + reg); // mov

    imm(imm64);
}

void Asm::MOV(Register write, Memory read) {
    if (write > R8) {
        m_dest[m_addr++] = CHAR(REX_B);
    }
    m_dest[m_addr++] = CHAR(0x8a);         // mov
    m_dest[m_addr++] = CHAR(0x00 + write * 8 + read); // [write], read
}

void Asm::MOV(Memory write, Register read) {
    m_dest[m_addr++] = CHAR(0x88);         // mov
    m_dest[m_addr++] = CHAR(0x00 + 8 * read + write); // [read], write
}

void Asm::CMP(Memory reg, int8_t imm8) {
    m_dest[m_addr++] = CHAR(0x80); // cmp
    m_dest[m_addr++] = CHAR(0x38 + reg); // opcode 7 + [register]

    imm(imm8);
}

void Asm::JNE(int32_t relativeAddress) {
    m_dest[m_addr++] = CHAR(0x0f); // 0F prefix
    m_dest[m_addr++] = CHAR(0x85); // jne

    imm(relativeAddress);
}


void Asm::JE(int32_t relativeAddress) {
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
    if (reg > R8) {
        m_dest[m_addr++] = CHAR(REX_B);
    }
    m_dest[m_addr++] = CHAR(0xff); // call
    m_dest[m_addr++] = CHAR(0xd0 + reg); // opcode 2 + reg
    ADD(RSP, 40);
}

void Asm::RET() {
    m_dest[m_addr++] = CHAR(0xc3); // ret
}

int32_t Asm::run() {
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