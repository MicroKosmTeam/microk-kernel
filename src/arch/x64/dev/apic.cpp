#include <arch/x64/dev/apic.hpp>
#include <sys/printk.hpp>
#include <arch/x64/cpu/cpu.hpp>
#include <init/kinfo.hpp>
#include <mm/vmm.hpp>

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

namespace x86_64::APIC {
inline bool IsAPICBSP() {
	u32 eax, edx;
	x86_64::GetMSR(IA32_APIC_BASE_MSR, &eax, &edx);
	
	return (eax & IA32_APIC_BASE_MSR_BSP);
}

inline uptr GetAPICBase() {
	u32 eax, edx;
	x86_64::GetMSR(IA32_APIC_BASE_MSR, &eax, &edx);

	return (eax & 0xfffff000) | (((uptr)edx & 0x0f) << 32);
}

inline void SetAPICBase(uptr apic) {
	u32 eax = (apic & 0xfffff0000) | IA32_APIC_BASE_MSR_ENABLE;
	u32 edx = (apic >> 32) & 0x0f;

	x86_64::SetMSR(IA32_APIC_BASE_MSR, eax, edx);
}


DEV::Device *CreateAPICDevice() {
	APIC *apic = new APIC;

	apic->Initialize = InitializeDevice;
	apic->Deinitialize = DeinitializeDevice;
	apic->Ioctl = Ioctl;
	
	return (DEV::Device*)apic;
}

int InitializeDevice(DEV::Device *device, va_list ap) {
	KInfo *info = GetInfo();
	APIC *apic = (APIC*)device;
	(void)ap;

	apic->Base = GetAPICBase();
	apic->MappedAddress = VMM::PhysicalToVirtual(apic->Base);
	VMM::MapPage(info->KernelVirtualSpace, apic->Base, apic->MappedAddress, VMM_FLAGS_KERNEL_DEVICE);
	SetAPICBase(apic->Base);

	ReadAPIC(apic, LAPICIDRegister, &apic->ID);
	apic->ProcessorIsBSP = IsAPICBSP();
	
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "APIC 0x%x (%s) at 0x%x\r\n", apic->ID, apic->ProcessorIsBSP ? "BSP" : "Not a BSP", apic->Base);

/*
	WriteAPICRegister(0xF0, 0x100 + 39);
	WriteAPICRegister(0x80, 0x1);

	APICTimer *timer = apic->Timer = new APICTimer;
	timer->Initialize = InitializeAPICTimer;

	WriteAPICRegister(0x320, 32);
	WriteAPICRegister(0x340, 0x10000);
	WriteAPICRegister(0x380, 0x0);

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "APIC enabled.\r\n");
*/
	return 0;
}

int DeinitializeDevice(DEV::Device *device, va_list ap) {
	APIC *apic = (APIC*)device;
	(void)apic;
	(void)ap;

	return 0;
}

intmax_t Ioctl(DEV::Device *device, request_t request, va_list ap) {
	APIC *apic = (APIC*)device;
	(void)apic;
	(void)request;
	(void)ap;

	return 0;
}


int ReadAPIC(APIC *device, APICRegisters registerSelector, u32 *value) {
	volatile u32 *apicRegister = (volatile u32*)(device->MappedAddress + registerSelector);

	*value = *apicRegister;
	return 0;
}

int WriteAPIC(APIC *device, APICRegisters registerSelector, u32 value) {
	volatile u32 *apicRegister = (volatile u32*)(device->MappedAddress + registerSelector);

	*apicRegister = value;
	return 0;
}

}
/*
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

void SendAPICEOI() {
	WriteAPICRegister(0xB0, 0x0);
	
}
*/
