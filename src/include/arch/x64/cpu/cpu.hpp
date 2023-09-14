/*
   File: include/arch/x64/cpu/cpu.hpp
*/

#pragma once
#include <stdint.h>

#define MSR_EFER         0xC0000080
#define MSR_STAR         0xC0000081
#define MSR_LSTAR        0xC0000082

#define MSR_FMASK        0xC0000084

#define MSR_FSBASE       0xC0000100
#define MSR_GSBASE       0xC0000101
#define MSR_KERNELGSBASE 0xC0000102

namespace x86_64 {
	struct LocalCPUStruct {
		uintptr_t TaskKernelStack;
		uintptr_t UserStackPointer;
		uintptr_t UserBaseStackPointer;
	}__attribute__((packed));

	inline void GetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi) {
		asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
	}
 
	inline void SetMSR(uint32_t msr, uint32_t lo, uint32_t hi) {
		asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
	}

	void CPUInit();
	void UpdateLocalCPUStruct(uintptr_t taskKernelStack);
	const char *GetCPUVendor();
}
