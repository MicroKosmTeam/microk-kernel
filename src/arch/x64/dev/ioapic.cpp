#include <arch/x64/dev/ioapic.hpp>
#include <mm/vmm.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>

#define IOAPIC_ENABLE_OFFSET 0x01

namespace x86_64 {
void EnableIOAPIC(uintptr_t address) {
	KInfo *info = GetInfo();

	VMM::MapMemory(info->kernelVirtualSpace, address, address);

	WriteIOAPICRegister(address, IOAPIC_ENABLE_OFFSET, 1);

	PRINTK::PrintK("IO APIC enabled\r\n");
}

uint64_t ReadIOAPICRegister(uintptr_t base, uint16_t offset) {
	return *((uint64_t*)(base + offset));
}

void WriteIOAPICRegister(uintptr_t base, uint16_t offset, uint64_t data) {
	*((uint64_t*)(base + offset)) = data;
}
}
