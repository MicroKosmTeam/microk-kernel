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
	 * as we will use it to initialize the TSS. We start from 640K in x86_64 until
	 * we allocate a proper location, as we know it will always be free for use.
	 */
	info->KernelStack = VMM::PhysicalToVirtual(640 * 1024); 
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Temporary kernel stack: 0x%x\r\n", info->KernelStack);

	/* Initialize the GDT */
	LoadGDT();

	/* Initialization of the TSS */
	TSSInit(info->KernelStack);
	FlushTSS();

	/* Jumpstart interrupts */
	IDTInit();
	
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Basic CPU structures initialized\r\n");
}

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
