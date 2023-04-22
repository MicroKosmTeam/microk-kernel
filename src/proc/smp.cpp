#include <proc/smp.hpp>

#ifdef CONFIG_MP_SMP

#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <sys/atomic.hpp>

namespace PROC {
namespace SMP {
void Rest(BootCPU *cpuInfo) {
	uint64_t cpu = cpuInfo->ProcessorID;
	PRINTK::PrintK("[CPU %d] Status: Resting.\r\n", cpu);

	while (true) {
		CPUPause();
	}
}

void Init() {
	KInfo *info = GetInfo();

	if (info->SMP.IsEnabled) {
		PRINTK::PrintK("Total CPU count: %d\r\n", info->SMP.CpuCount);
		for (int i = 1; i < info->SMP.CpuCount; i++) {
			PRINTK::PrintK("Starting CPU %d...\r\n", i);
			
			*info->SMP.Cpus[i].GotoAddress = Rest;
		}
	} else {
		PRINTK::PrintK("No multiprocessing available.\r\n");
	}
}
}
}

#endif