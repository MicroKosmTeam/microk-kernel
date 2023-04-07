#include <proc/smp.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <sys/atomic.hpp>

namespace PROC {
void SMPRest(BootCPU *cpuInfo) {
	uint64_t cpu = cpuInfo->ProcessorID;
	PRINTK::PrintK("[CPU %d] Status: Active.\r\n", cpu);

	while (true) CPUPause();
}

void InitSMP() {
	KInfo *info = GetInfo();

	if (info->SMP.IsEnabled) {
		PRINTK::PrintK("Total CPU count: %d\r\n", info->SMP.CpuCount);
		for (int i = 1; i < info->SMP.CpuCount; i++) {
			PRINTK::PrintK("Starting CPU %d...\r\n", i);
			
			*info->SMP.Cpus[i].ExtraArgument = 0x1;
			*info->SMP.Cpus[i].GotoAddress = SMPRest;
		}
	} else {
		PRINTK::PrintK("No multiprocessing available.\r\n");
	}
}
}
