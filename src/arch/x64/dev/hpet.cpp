#include <arch/x64/dev/acpi.hpp>

namespace x86_64 {
void WriteHPETRegister(uintptr_t baseAddress, uint32_t offset, uint32_t value)
{
    *(volatile uint32_t*)(baseAddress + offset) = value;
}
}
