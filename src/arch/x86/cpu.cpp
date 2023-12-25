#include <cpuid.h>
#include <printk.hpp>
#include <memory.hpp>
#include <arch/x86/cpu.hpp>
#include <arch/x86/idt.hpp>
#include <arch/x86/gdt.hpp>
#include <arch/x86/apic.hpp>

extern "C" void SyscallEntry();

void SyscallEntry() {

}

namespace x86 {
GDT gdt;
GDTPointer pointer;
TSS tss;
APIC apic;


inline static u32 GetFamily(u32 sig) {
	u32 x86;

	x86 = (sig >> 8) & 0xf;

	if (x86 == 0xf)
		x86 += (sig >> 20) & 0xff;

	return x86;
}

inline static u32 GetModel(u32 sig) {
	u32 fam, model;

	fam = GetFamily(sig);

	model = (sig >> 4) & 0xf;

	if (fam >= 0x6)
		model += ((sig >> 16) & 0xf) << 4;

	return model;
}

inline static u32 GetStepping(u32 sig) {
	return sig & 0xf;
}

inline static int EnableSyscalls() {
	PRINTK::PrintK(PRINTK_DEBUG "Syscall entries at 0x%x\r\n", &SyscallEntry);

	u32 msrLo = 0, msrHi = 0;

	/* Enabling syscalls with MSRs, first starting with the SCE enable bit (0) in the EFER MSR */
	GetMSR(MSR_EFER, &msrLo, &msrHi);
	msrLo |= 1;
	SetMSR(MSR_EFER, msrLo, msrHi);

	/* Settign the higher part of the STAR MSR with the GDT offsets (low part is for EIP, unused in 64 bit mode */
	msrLo = 0;
	msrHi = (GDT_OFFSET_KERNEL_CODE) | (GDT_OFFSET_USER_CODE << 16);
	SetMSR(MSR_STAR, msrLo, msrHi);

	/* Setting syscall RIP for 64 bit mode in the LSTAR MSR */
	msrLo = (uptr)&SyscallEntry;
	msrHi = (uptr)&SyscallEntry >> 32;
	SetMSR(MSR_LSTAR, msrLo, msrHi);

	/* Setting syscall RIP for compatibility mode in the CSTAR MSR (for now it's 0, not supported */
	msrLo = 0xDEADC0DE;
	msrHi = 0x0;
	SetMSR(MSR_CSTAR, msrLo, msrHi);

	/* Setting RFLAGS mask to 0 in the FMASK MSR, no masking necessary */
	msrLo = 0;
	msrHi = 0;
	SetMSR(MSR_FMASK, msrLo, msrHi);

	return 0;
}

void LoadEssentialCPUStructures() {
	x86::LoadGDT(&gdt, &pointer);
	x86::TSSInit(&gdt, &tss, 0);
	x86::IDTInit();
}


void InitializeCPUFeatures() {
	EnableSyscalls();

	u32 vendor[4];
	Memclr(vendor, sizeof(u32) * 4);
	u32 maxIntelLevel = 0, maxAmdLevel = 0;
	u32 ignored = 0;
	__get_cpuid_count(0, 0, &maxIntelLevel, &vendor[0], &vendor[2], &vendor[1]);
	__get_cpuid_count(0x80000000, 0, &maxAmdLevel, &ignored, &ignored, &ignored);

	PRINTK::PrintK(PRINTK_DEBUG 
			"CPUID max supported Intel level: 0x%x\r\n"
			"CPUID max supported AMD level: 0x%x\r\n"
			"CPUID vendor string: %s\r\n", maxIntelLevel, maxAmdLevel, vendor);

	InitializeAPIC(&apic);

	/*
	if (maxIntelLevel >= 0x00000001 &&
	    maxIntelLevel <= 0x0000ffff) {
		u32 vendorInfo = 0;
		__get_cpuid_count(1, 0, &vendorInfo, &feature, &coreInfo->CPUIDFlags[0], &coreInfo->CPUIDFlags[1]);

		u32 family, model, stepping;
		family = GetFamily(vendorInfo);
		model = GetModel(vendorInfo);
		stepping = GetStepping(vendorInfo);

		PRINTK::PrintK(PRINTK_DEBUG "CPUID processor info:\r\n"
			"Family:   0x%x\r\n"
			"Model:    0x%x\r\n"
			"Stepping: 0x%x\r\n"
			, family, model, stepping);

		apicID = (feature & 0xFF000000) >> 24;
	}

	// To get x2APIC data
	if (maxIntelLevel >= 0x0000000b) {
		__get_cpuid_count(0x0000000b, 0, &ignored, &ignored, &ignored, &feature);

		if (feature != apicID && feature != 0) {
			apicID = feature;
		}
	}

	if (maxAmdLevel >= 0x80000001 &&
	    maxAmdLevel <= 0x8000ffff) {
		__get_cpuid_count(0x80000001, 0, &ignored, &ignored, &feature, &feature);
	}

	core->ID = apicID;

	if (coreInfo->CPUIDFlags[1] & FLAGS_1_IS_XAPIC_PRESENT) {
		bool isX2APIC;

		if (coreInfo->CPUIDFlags[0] & FLAGS_0_IS_X2APIC_PRESENT) {
			isX2APIC = true;
		} else {
			isX2APIC = false;
		}
	
		APIC::APIC *localAPIC = (APIC::APIC*)APIC::CreateAPICDevice();
		coreInfo->LocalAPIC = localAPIC;

		DEV::InitializeDevice((DEV::Device*)localAPIC, isX2APIC);
	} else {
		PANIC("No local APIC found");
	}

	coreInfo->TimeStampCounter = (TSC::TSC*)TSC::CreateTSCDevice();

	SetIOPL();

	SetMSR(MSR_GSBASE, 0, 0);

	SetMSR(MSR_KERNELGSBASE, (uptr)&coreInfo->CPUStruct, (uptr)&coreInfo->CPUStruct >> 32);
	UpdateLocalCPUStruct(&coreInfo->CPUStruct, 0, VMM::VirtualToPhysical(info->KernelVirtualSpace->VirtualHierarchyTop), VMM::VirtualToPhysical(info->KernelVirtualSpace->VirtualHierarchyTop));

	coreInfo->CPUStruct.TopologyStructure = (uptr)core;*/
}

void InitializeBootCPU() {
	LoadEssentialCPUStructures();
}
}