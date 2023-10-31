/*
   File: arch/x64/main.cpp
*/

#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <arch/x64/main.hpp>
#include <arch/x64/cpu/cpu.hpp>
#include <arch/x64/cpu/gdt.hpp>
#include <arch/x64/dev/main.hpp>
#include <arch/x64/interrupts/idt.hpp>

namespace x86_64 {
void Init() {
	KInfo *info = GetInfo();

	info->KernelStack = VMM::PhysicalToVirtual((uptr)PMM::RequestPages(KERNEL_STACK_SIZE / PAGE_SIZE) + KERNEL_STACK_SIZE);
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "CPU interrupt stack: 0x%x\r\n", info->KernelStack);

	/* Initialization of the TSS */
	TSSInit(info->KernelStack);

	/* x86 CPU initialization */
	CPUInit();

	/* We get the CPU vendor */
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "CPU Vendor: %s\r\n", GetCPUVendor());

	InitDevices();
}
}
