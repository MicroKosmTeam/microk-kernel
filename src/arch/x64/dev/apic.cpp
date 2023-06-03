#include <arch/x64/dev/apic.hpp>
#include <sys/printk.hpp>
#include <arch/x64/cpu/cpu.hpp>
#include <init/kinfo.hpp>
#include <mm/vmm.hpp>

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

namespace x86_64 {
uintptr_t GetAPICBase() {
	uint32_t eax, edx;
	GetMSR(IA32_APIC_BASE_MSR, &eax, &edx);

	return (eax & 0xfffff000) | ((edx & 0x0f) << 32);
}

void SetAPICBase(uintptr_t apic) {
	uint32_t eax = (apic & 0xfffff0000) | IA32_APIC_BASE_MSR_ENABLE;
	uint32_t edx = (apic >> 32) & 0x0f;

	SetMSR(IA32_APIC_BASE_MSR, eax, edx);
}

void WriteAPICRegister(uint16_t offset, uint32_t data) {
	uintptr_t apic = GetAPICBase();

	uint32_t *apicRegister = (uint32_t*)(apic + offset);

	*apicRegister = data;
}

uint32_t ReadAPICRegister(uint16_t offset) {
	uintptr_t apic = GetAPICBase();

	uint32_t *apicRegister = (uint32_t*)(apic + offset);

	return *apicRegister;
}

void WaitAPIC(uint32_t cycles) {
	WriteAPICRegister(0x380, cycles);
}

void EnableAPIC() {	
	KInfo *info = GetInfo();
	void *base = GetAPICBase();

	VMM::MapMemory(info->kernelVirtualSpace, base, base);

	SetAPICBase(GetAPICBase());

	PRINTK::PrintK("APIC at 0x%x\r\n", GetAPICBase());

	WriteAPICRegister(0xF0, 0x100 + 39);
	WriteAPICRegister(0x80, 0x1);

	WriteAPICRegister(0x320, 32);
	WriteAPICRegister(0x340, 0x10000);
	WriteAPICRegister(0x380, 0x0);

	PRINTK::PrintK("APIC enabled.\r\n");
}

void SetAPICTimer() {
	WaitAPIC(0x10000);
}

void SendAPICEOI() {
	WriteAPICRegister(0xB0, 0x0);
	
}
}
