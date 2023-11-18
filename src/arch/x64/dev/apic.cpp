#include <arch/x64/dev/apic.hpp>
#include <sys/printk.hpp>
#include <arch/x64/cpu/cpu.hpp>
#include <init/kinfo.hpp>
#include <mm/vmm.hpp>

#define MSR_APIC_FLAG_IS_BSP        (1 << 8)
#define MSR_APIC_FLAG_x2APIC_ENABLE (1 << 10)
#define MSR_APIC_FLAG_xAPIC_ENABLE  (1 << 11)

namespace x86_64::APIC {
inline __attribute__((always_inline))
void ReadX2APIC(APIC *device, usize registerSelector, u32 *lo, u32 *hi) {
	(void)device;
	GetMSR(MSR_x2APIC_BEGIN + registerSelector, lo, hi);
}

inline __attribute__((always_inline))
void ReadX2APIC(APIC *device, usize registerSelector, u32 *lo) {
	u32 ignore;
	ReadX2APIC(device, registerSelector, lo, &ignore);
}


inline __attribute__((always_inline))
void WriteX2APIC(APIC *device, usize registerSelector, u32 lo, u32 hi) {
	(void)device;
	SetMSR(MSR_x2APIC_BEGIN + registerSelector, lo, hi);
}

inline __attribute__((always_inline))
void WriteX2APIC(APIC *device, usize registerSelector, u32 lo) {
	WriteX2APIC(device, registerSelector, lo, 0);
}

inline __attribute__((always_inline))
void ReadXAPIC(APIC *device, usize registerSelector, u32 *lo, u32 *hi) {
	volatile u32 *apicRegister = (volatile u32*)(device->MappedAddress + registerSelector);

	*lo = *apicRegister;
	*hi = *(apicRegister + 1);
}


inline __attribute__((always_inline))
void ReadXAPIC(APIC *device, usize registerSelector, u32 *lo) {
	volatile u32 *apicRegister = (volatile u32*)(device->MappedAddress + registerSelector);

	*lo = *apicRegister;
}

inline __attribute__((always_inline))
void WriteXAPIC(APIC *device, usize registerSelector, u32 lo, u32 hi) {
	volatile u32 *apicRegister = (volatile u32*)(device->MappedAddress + registerSelector);

	*apicRegister = lo;
	*(apicRegister + 1) = hi;
}

inline __attribute__((always_inline))
void WriteXAPIC(APIC *device, usize registerSelector, u32 lo) {
	volatile u32 *apicRegister = (volatile u32*)(device->MappedAddress + registerSelector);

	*apicRegister = lo;
}

inline __attribute__((always_inline))
bool IsAPICBSP() {
	u32 eax, edx;
	x86_64::GetMSR(MSR_APIC_BASE, &eax, &edx);
	
	return (eax & MSR_APIC_FLAG_IS_BSP);
}

inline __attribute__((always_inline))
uptr GetAPICBase() {
	u32 eax, edx;
	x86_64::GetMSR(MSR_APIC_BASE, &eax, &edx);

	return (eax & 0xfffff000) | (((uptr)edx & 0x0f) << 32);
}

inline __attribute__((always_inline))
void SetAPICBase(uptr apic, usize flags) {
	u32 eax = (apic & 0xfffff0000) | flags;
	u32 edx = (apic >> 32) & 0x0f;

	x86_64::SetMSR(MSR_APIC_BASE, eax, edx);
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
	VMM::MapPage(info->KernelVirtualSpace, apic->Base, apic->MappedAddress, VMM_FLAGS_KERNEL_DATA);
	
	ReadXAPIC(apic, xAPIC_REGISTER_LAPIC_ID, &apic->ID);

	apic->ProcessorIsBSP = IsAPICBSP();
	
	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "APIC 0x%x (%s) at 0x%x\r\n", apic->ID, apic->ProcessorIsBSP ? "BSP" : "Not a BSP", apic->Base);
	
	apic->x2APICMode = true;
	SetAPICBase(apic->Base, MSR_APIC_FLAG_xAPIC_ENABLE | MSR_APIC_FLAG_x2APIC_ENABLE);

	u8 timerVector = 32;
	u32 timer = timerVector | APIC_LVT_TIMER_TSCDEADLINE | APIC_LVT_TIMER_MASK;
	u32 spuriousVector = 33 | 0x100;

	if (apic->x2APICMode) {
		WriteX2APIC(apic, x2APIC_REGISTER_TASK_PRIORITY, 0x1);
		WriteX2APIC(apic, x2APIC_REGISTER_SPURIOUS_INTERRUPT_VECTOR, spuriousVector);

		WriteX2APIC(apic, x2APIC_REGISTER_TIMER_INITIAL_COUNT, -1);
		WriteX2APIC(apic, x2APIC_REGISTER_TIMER_DIVIDE_CONFIGURATION, APIC_LVT_TIMER_DIVIDE_1);
		WriteX2APIC(apic, x2APIC_REGISTER_LVT_TIMER_REGISTER, timer);

		u32 ignore;
		ReadX2APIC(apic, x2APIC_REGISTER_TIMER_CURRENT_COUNT, &ignore);

		ReadX2APIC(apic, x2APIC_REGISTER_LVT_TIMER_REGISTER, &timer);
		timer &= ~APIC_LVT_TIMER_MASK;
		WriteX2APIC(apic, x2APIC_REGISTER_LVT_TIMER_REGISTER, timer);
	} else {
		WriteXAPIC(apic, xAPIC_REGISTER_TASK_PRIORITY, 0x1);
		WriteXAPIC(apic, xAPIC_REGISTER_SPURIOUS_INTERRUPT_VECTOR, spuriousVector);

		WriteXAPIC(apic, xAPIC_REGISTER_TIMER_INITIAL_COUNT, -1);
		WriteXAPIC(apic, xAPIC_REGISTER_TIMER_CURRENT_COUNT, 0);
		WriteXAPIC(apic, xAPIC_REGISTER_TIMER_DIVIDE_CONFIGURATION, APIC_LVT_TIMER_DIVIDE_1);
		WriteXAPIC(apic, xAPIC_REGISTER_LVT_TIMER_REGISTER, timer);

		u32 ignore;
		ReadXAPIC(apic, xAPIC_REGISTER_TIMER_CURRENT_COUNT, &ignore);

		ReadXAPIC(apic, xAPIC_REGISTER_LVT_TIMER_REGISTER, &timer);
		timer &= ~APIC_LVT_TIMER_MASK;
		WriteXAPIC(apic, xAPIC_REGISTER_LVT_TIMER_REGISTER, timer);

	}
	
	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "APIC enabled.\r\n");

	/*
	u64 tsc = __builtin_ia32_rdtsc() + 0x100000000;
	SetMSR(MSR_TSC_DEADLINE, tsc & 0xFFFFFFFF, tsc >> 32);

	while(true);

	WriteXAPIC(apic, xAPIC_REGISTER_EOI, 0);
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
}
