#pragma once
#include <object.hpp>

#define MSR_TSC_VALUE    0x00000010
#define MSR_APIC_BASE    0x0000001B
#define MSR_TSC_ADJUST   0x0000003B
#define MSR_TSC_MPERF    0x000000E7
#define MSR_TSC_APERF    0x000000E8
#define MSR_TSC_AUX      0x00000103
#define MSR_MISC_ENABLE  0x000001A0
#define MSR_PAT          0x00000277
#define MSR_TSC_DEADLINE 0x000006E0

#define MSR_x2APIC_BEGIN 0x00000800
#define MSR_x2APIC_END   0x000008FF

#define MSR_EFER         0xC0000080
#define MSR_STAR         0xC0000081
#define MSR_LSTAR        0xC0000082
#define MSR_CSTAR        0xC0000083
#define MSR_FMASK        0xC0000084
#define MSR_FSBASE       0xC0000100
#define MSR_GSBASE       0xC0000101
#define MSR_KERNELGSBASE 0xC0000102

// SVM
#define MSR_VM_CR        0xC0010114
#define MSR_VM_HSAVE_PA  0xC0010117

#define MSR_KVM_SYSTEM_TIME_NEW 0x4b564d01
struct PVClockTimeInfo_t {
	u32 version;
	u32 pad0;
	u64 TSCTimestamp;
	u64 system_time;
	u32 tsc_to_system_mul;
	u8 tsc_shift;
	u8 flags;
	u8 pad[2];
}__attribute__((packed));

namespace x86 {
        inline __attribute__((always_inline))
        void GetMSR(u32 msr, u32 *lo, u32 *hi) {
                asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
        }
 
        inline __attribute__((always_inline))
        void SetMSR(u32 msr, u32 lo, u32 hi) {
                asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
        }

        inline __attribute__((always_inline))
	u64 GetCR0() {
		u64 cr0;
		__asm__ volatile ("mov %0, %%cr0" : "=r" (cr0));
		return cr0;
	}

        inline __attribute__((always_inline))
	u64 GetCR2() {
		u64 cr2;
		__asm__ volatile ("mov %0, %%cr2" : "=r" (cr2));
		return cr2;
	}

        inline __attribute__((always_inline))
	u64 GetCR3() {
		u64 cr3;
		__asm__ volatile ("mov %0, %%cr3" : "=r" (cr3));
		return cr3;
	}

        inline __attribute__((always_inline))
	u64 GetCR4() {
		u64 cr4;
		__asm__ volatile ("mov %0, %%cr4" : "=r" (cr4));
		return cr4;
	}

        inline __attribute__((always_inline))
	u64 GetRFLAGS() {
		u64 rflags;
		__asm__ volatile (
			"pushfq\n"            // Push RFLAGS onto the stack
			"pop %0"              // Pop it into the rflags variable
			: "=r" (rflags)
		);
		return rflags;
	}

	extern "C"
	u64 GetRSP();

	extern "C"
	u64 GetRIP();

	struct _DTR {
		u16 Limit;
#if defined(__x86_64__)
		u64 Base;
#else
		u32 Base;
#endif
	} __attribute__((packed));

        inline __attribute__((always_inline))
	void GetGDTR(_DTR *gdtr) {
		__asm__ volatile (
			"sgdt %0"
			: "=m" (*gdtr)
		);
	}

        inline __attribute__((always_inline))
	void GetIDTR(_DTR *idtr) {
		__asm__ volatile (
			"sidt %0"
			: "=m" (*idtr)
		);
	}

	inline __attribute__((always_inline))
	u16 GetCS() {
		u16 cs;
		__asm__ volatile (
			"mov %0, %%cs"
			: "=r" (cs)
		);

		return cs;
	}

        inline __attribute__((always_inline))
	u16 GetDS() {
		u16 ds;
		__asm__ volatile (
			"mov %0, %%ds"
			: "=r" (ds)
		);
		
		return ds;
	}

        inline __attribute__((always_inline))
	u16 GetES() {
		u16 es;
		__asm__ volatile (
			"mov %0, %%es"
			: "=r" (es)
		);
	
		return es;
	}

        inline __attribute__((always_inline))
	u16 GetFS() {
		u16 fs;
		__asm__ volatile (
			"mov %0, %%fs"
			: "=r" (fs)
		);

		return fs;
	}

        inline __attribute__((always_inline))
	u16 GetGS() {
		u16 gs;
		__asm__ volatile (
			"mov %0, %%gs"
			: "=r" (gs)
		);

		return gs;
	}

	inline __attribute__((always_inline))
	u16 GetSS() {
		u16 ss;
		__asm__ volatile (
			"mov %0, %%ss"
			: "=r" (ss)
		);

		return ss;
	}

	void LoadEssentialCPUStructures();
	void InitializeCPUFeatures();

	void InitializeBootCPU();

	__attribute__((noreturn))
	void LoadSchedulerContext(SchedulerContext *context);

}
