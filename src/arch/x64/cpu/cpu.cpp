/*
   File: arch/x64/cpu/cpu.cpp
*/

#include <cpuid.h>
#include <sys/printk.hpp>
#include <arch/x64/cpu/cpu.hpp>
#include <sys/panic.hpp>
#include <stdint.h>

extern "C" void SyscallEntry();

extern "C" void EnableSCE(void *syscallEntrypoint);
extern "C" void EnableSSE();
extern "C" void EnableAVX();


static uint16_t EnableSMID() {
	unsigned int eax, ebx, ecx, edx;
	__cpuid(1, eax, ebx, ecx, edx);

	uint16_t result = 0;

	/* SSE */
	if(edx & (1 << 25)) {
		/* Enable SSE */
		EnableSSE();

		result |= (1 << 0);

		/* SSE2 */
		if(edx & (1 << 26)) {
			result |= (1 << 1);
		}

		/* SSE3 */
		if(ecx & (1 << 0)) {
			result |= (1 << 2);

			/* SSSE3 */
			if(ecx & (1 << 9)) {
				result |= (1 << 3);
			}
		}

		/* SSE4.1 */
		if(ecx & (1 << 19)) {
			result |= (1 << 4);

			/* SSE4.2 */
			if(ecx & (1 << 20)) {
				result |= (1 << 5);
			}

			/* SSE4A */
			if(ecx & (1 << 6)) {
				result |= (1 << 6);
			}
		}
	

		/* XSAVE */
		if(ecx & (1 << 26)) {
//			Not yet decided
			result |= (1 << 7);

	
		/* AVX */
		if(ecx & (1 << 28)) {
//			Not yet decided
			result |= (1 << 8);

			EnableAVX();
		}

		}
	}

	return result;
}

namespace x86_64 {
/*
   Function that initializes the x86CPU class
*/
void x86CPU::Init() {
	uint16_t sseFeatures = EnableSMID();
	PRINTK::PrintK("SSE features: %x\r\n", sseFeatures);

	PRINTK::PrintK("Syscall entry at 0x%x\r\n", &SyscallEntry);
	EnableSCE((void*)&SyscallEntry);
}

/*
   Function that gets the CPU vendor string from CPUID
*/
void x86CPU::GetVendor(char *string) {
	uint32_t ebx, edx, ecx, unused;
	__cpuid(0, unused, ebx, ecx, edx);

	/* This bitshift logic is used to get the correct 8-bit characters
	   from the 32 bit registers. The vendor name is 12 characters + \0 */
	string[0] = (uint8_t)ebx;
	string[1] = (uint8_t)(ebx >> 8);
	string[2] = (uint8_t)(ebx >> 16);
	string[3] = (uint8_t)(ebx >> 24);
	string[4] = (uint8_t)edx;
	string[5] = (uint8_t)(edx >> 8);
	string[6] = (uint8_t)(edx >> 16);
	string[7] = (uint8_t)(edx >> 24);
	string[8] = (uint8_t)ecx;
	string[9] = (uint8_t)(ecx >> 8);
	string[10] = (uint8_t)(ecx >> 16);
	string[11] = (uint8_t)(ecx >> 24);
	string[12] = '\0';
}
}
