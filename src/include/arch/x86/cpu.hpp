#pragma once
#include <cstdint.hpp>
#include <object.hpp>

#define MSR_TSC_VALUE    0x00000010
#define MSR_APIC_BASE    0x0000001B
#define MSR_TSC_ADJUST   0x0000003B
#define MSR_TSC_MPERF    0x000000E7
#define MSR_TSC_APERF    0x000000E8
#define MSR_TSC_AUX      0x00000103
#define MSR_MISC_ENABLE  0x000001A0
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

namespace x86 {
        inline __attribute__((always_inline))
        void GetMSR(u32 msr, u32 *lo, u32 *hi) {
                asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
        }
 
        inline __attribute__((always_inline))
        void SetMSR(u32 msr, u32 lo, u32 hi) {
                asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
        }

	void LoadEssentialCPUStructures();
	void InitializeCPUFeatures();

	void InitializeBootCPU();

	__attribute__((noreturn))
	void GoToUserspace(SchedulerContext *context);

}
