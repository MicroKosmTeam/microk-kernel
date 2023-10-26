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
void EarlyInit() {
	KInfo *info = GetInfo();

	/* We first of all get the position of the kernel interrupt stack and save it
	 * as we will use it to initialize the TSS.
	 */
	//info->KernelStack = (uptr)PMM::RequestPages(KERNEL_STACK_SIZE / PAGE_SIZE) + info->HigherHalfMapping;
	info->KernelStack =  0x80000 + info->HigherHalfMapping;
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Kernel stack: 0x%x\r\n", info->KernelStack);

	/* Initialize the GDT */
	LoadGDT();
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "GDT Loaded.\r\n");

	/* Initialization of the TSS */
	TSSInit(info->KernelStack);
	FlushTSS();

	/* Jumpstart interrupts */
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Loading x86_64 IDT\r\n");
	IDTInit();
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "IDT Loaded.\r\n");
}

void Init() {
	/* x86 CPU initialization */
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Setting up CPU features\r\n");
	CPUInit();

	/* We get the CPU vendor */
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "CPU Vendor: %s\r\n", GetCPUVendor());

	InitDevices();
}
}
