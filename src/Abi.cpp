

#include "Abi.h"

Register WindowsX64Abi::firstParameterRegister() {
    return RCX;
}

Register *WindowsX64Abi::freeRegisters() {
    static Register registers[] = {RBX, RSI, RDI};
    return registers;
}

// ------------------------------------

Register LinuxX64Abi::firstParameterRegister() {
    return RDI;
}

Register *LinuxX64Abi::freeRegisters() {
    static Register registers[] = {RBX, R12, R13};
    return registers;
}
