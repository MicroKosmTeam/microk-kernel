/*
   File: include/arch/x64/cpu/cpu.hpp
*/

#pragma once
#include <stdint.h>

namespace x86_64 {
	/* This class is used to manage an x86 CPU */
	class x86CPU {
	public:
		x86CPU() {
			Init();
		}

		/* Function that initializes the struct */
		void Init();

		/* Declared inline for faster execution
		   as it is run in functions like memcpy */
		inline bool IsSSE() {
			return sseStatus;
		}

		/* Function that returns the CPU vendor string */
		void GetVendor(char *string);
	private:
		/* Wether SSE is actually active */
		bool sseStatus = false;
	};

	inline void GetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi) {
		asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
	}
 
	inline void SetMSR(uint32_t msr, uint32_t lo, uint32_t hi) {
		asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
	}
}
