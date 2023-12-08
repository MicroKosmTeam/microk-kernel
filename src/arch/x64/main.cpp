/*
   File: arch/x64/main.cpp
*/

#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <arch/x64/main.hpp>
#include <arch/x64/cpu/cpu.hpp>
#include <arch/x64/cpu/gdt.hpp>
#include <arch/x64/interrupts/idt.hpp>

namespace x86_64 {
void Init() {
	KInfo *info = GetInfo();

	/* x86 CPU initialization */
	UpdatePerCPUStack(info->BootCore, KERNEL_STACK_SIZE);
	CurrentCPUInit(info->BootCore);
}
}
