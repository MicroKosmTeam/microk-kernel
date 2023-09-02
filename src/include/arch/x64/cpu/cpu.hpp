/*
   File: include/arch/x64/cpu/cpu.hpp
*/

#pragma once
#include <stdint.h>

namespace x86_64 {
	inline void GetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi) {
		asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
	}
 
	inline void SetMSR(uint32_t msr, uint32_t lo, uint32_t hi) {
		asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
	}

	void CPUInit();
	const char *GetCPUVendor();
}
