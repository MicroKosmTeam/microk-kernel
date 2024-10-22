#include <cpu.hpp>
#include <pmm.hpp>
#include <kinfo.hpp>
#include <capability.hpp>

#if defined(__x86_64__)
#include <arch/x86/cpu.hpp>
#elif defined(__aarch64__)
#include <arch/aarch64/cpu.hpp>
#endif

namespace ARCH {
void InitializeBootCPU() {
#if defined(__x86_64__)
	x86::InitializeBootCPU();
#elif defined(__aarch64__)
	AArch64::InitializeBootCPU();
#endif
}

void InitializeCPUFeatures() {
	KInfo *info = GetInfo();

	// TODO 
	info->BootDomain = (Domain*)PMM::RequestPage();
	Memclr(info->BootDomain, sizeof(Domain));

#if defined(__x86_64__)
	x86::InitializeCPUFeatures();
#endif
}

__attribute__((noreturn))
void LoadSchedulerContext(SchedulerContext *context) {
#if defined(__x86_64__)
	x86::LoadSchedulerContext(context);
#else
	/* TODO */
	while(true) {}
	(void)context;
#endif

}

}
