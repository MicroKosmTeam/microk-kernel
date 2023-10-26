#include <arch/x64/dev/apic.hpp>
#include <sys/printk.hpp>
#include <arch/x64/cpu/cpu.hpp>
#include <init/kinfo.hpp>
#include <mm/vmm.hpp>

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

namespace x86_64 {
uptr GetAPICBase() {
	u32 eax, edx;
	GetMSR(IA32_APIC_BASE_MSR, &eax, &edx);

	return (eax & 0xfffff000) | (((uptr)edx & 0x0f) << 32);
}

void SetAPICBase(uptr apic) {
	u32 eax = (apic & 0xfffff0000) | IA32_APIC_BASE_MSR_ENABLE;
	u32 edx = (apic >> 32) & 0x0f;

	SetMSR(IA32_APIC_BASE_MSR, eax, edx);
}

void WriteAPICRegister(u16 offset, u32 data) {
	KInfo *info = GetInfo();
	uptr apic = GetAPICBase() + info->HigherHalfMapping;

	volatile u32 *apicRegister = (volatile u32*)(apic + offset);

	*apicRegister = data;
}

u32 ReadAPICRegister(u16 offset) {
	KInfo *info = GetInfo();
	uptr apic = GetAPICBase() + info->HigherHalfMapping;

	volatile u32 *apicRegister = (volatile u32*)(apic + offset);

	return *apicRegister;
}
	
static u64 cycles = 0;
void SetAPICTimer(u64 newCycles) {
	cycles = newCycles;
}

void WaitAPIC() {
	WriteAPICRegister(0x380, cycles);
}
	
int InitializeAPICTimer(TIME::Timer *timer) {
	(void)timer;

	return 0;
}

void EnableAPIC() {	
	APIC *apic = new APIC;
	apic->Initialize = NULL;
	apic->Deinitialize = NULL;
	apic->Ioctl = NULL;

	apic->Base = GetAPICBase();

	SetAPICBase(apic->Base);
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "APIC at 0x%x\r\n", apic->Base);

	WriteAPICRegister(0xF0, 0x100 + 39);
	WriteAPICRegister(0x80, 0x1);

	APICTimer *timer = apic->Timer = new APICTimer;
	timer->Initialize = InitializeAPICTimer;

	WriteAPICRegister(0x320, 32);
	WriteAPICRegister(0x340, 0x10000);
	WriteAPICRegister(0x380, 0x0);

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "APIC enabled.\r\n");
}

void SendAPICEOI() {
	WriteAPICRegister(0xB0, 0x0);
	
}
}
