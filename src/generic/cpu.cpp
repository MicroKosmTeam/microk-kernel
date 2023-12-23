#include <cpu.hpp>

#if defined(__x86_64__)
#include <arch/x86/cpu.hpp>
#else
#endif

namespace ARCH {
void InitializeBootCPU() {
#if defined(__x86_64__)
	x86::InitializeBootCPU();
#else
#endif
}

void InitializeCPUFeatures() {
#if defined(__x86_64__)
	x86::InitializeCPUFeatures();
#else
#endif
}

}
