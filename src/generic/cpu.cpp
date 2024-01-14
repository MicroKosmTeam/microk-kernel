#include <cpu.hpp>
#include <kinfo.hpp>
#include <pmm.hpp>
#include <capability.hpp>

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
	KInfo *info = GetInfo();

	info->BootDomain = (Domain*)VMM::PhysicalToVirtual((uptr)PMM::RequestPage());

	CAPABILITY::Originate(info->RootCapabilitySpace, (uptr)info->BootDomain, sizeof(Domain), ObjectType::DOMAIN, CapabilityRights::NONE);

#if defined(__x86_64__)
	x86::InitializeCPUFeatures();
#else
#endif
}

__attribute__((noreturn))
void LoadSchedulerContext(SchedulerContext *context) {
#if defined(__x86_64__)
	x86::LoadSchedulerContext(context);
#else
#endif

}

}
