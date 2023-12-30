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

__attribute__((noreturn))
void GoToUserspace(SchedulerContext *context) {
#if defined(__x86_64__)
	x86::GoToUserspace(context);
#else
#endif

}

}
