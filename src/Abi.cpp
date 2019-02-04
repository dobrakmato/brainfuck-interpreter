

#include "Abi.h"

Register WindowsX64Abi::firstParameterRegister() {
    return RCX;
}

Register LinuxX64Abi::firstParameterRegister() {
    return RDI;
}
