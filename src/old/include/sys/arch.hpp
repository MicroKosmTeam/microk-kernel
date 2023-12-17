#pragma once
#include <cdefs.h>

#if defined(__x86_64__)
#include <arch/x64/main.hpp>
#include <arch/x64/cpu/cpu.hpp>
#elif defined(__aarch64__)
#include <arch/aarch64/main.hpp>
#endif

namespace ARCH {
	inline __attribute__((always_inline))
	int SetupArch() {
		#if defined(__x86_64__)
			x86_64::Init();
		#elif defined(__aarch64__)
			AArch64::Init();
		#endif
		
		return 0;
	}
	
	inline __attribute__((always_inline))
	uptr GetCurrentStackPointer() {
		#if defined(__x86_64__)
			return x86_64::GetCurrentStackPointer();
		#elif defined(__aarch64__)
			return AArch64::GetCurrentStackPointer();
		#endif

		return -1;
	}
}

