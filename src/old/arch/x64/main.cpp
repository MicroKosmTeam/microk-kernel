/*
   File: arch/x64/main.cpp
*/

#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <arch/x64/main.hpp>
#include <arch/x64/dev/main.hpp>
#include <arch/x64/cpu/cpu.hpp>
#include <arch/x64/cpu/gdt.hpp>
#include <arch/x64/interrupts/idt.hpp>

namespace x86_64 {
void Init() {
	KInfo *info = GetInfo();

	/* x86 CPU initialization */
	UpdatePerCPUStack(info->BootCore, KERNEL_STACK_SIZE);
	CurrentCPUInit(info->BootCore);

/*
	DEV::CPU::TopologyStructure *machine = info->DefaultMachine;
	PerMachineTopology *machineInfo = (PerMachineTopology*)machine->ArchitectureSpecificInformation;
	
	if (machineInfo->ACPI_TableSRAT!= 0) {
		HandleSRAT((SRATHeader*)machineInfo->ACPI_TableSRAT);
	}
	if (machineInfo->ACPI_TableMADT != 0) {
		HandleMADT((MADTHeader*)machineInfo->ACPI_TableMADT);
	}
*/
}
}
