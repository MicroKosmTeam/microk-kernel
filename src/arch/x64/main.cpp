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
	 * as we will use it to initialize the TSS
	 *
	 * We use 0x800000 because it's a low memory address that is
	 * the highest that is always (hopefully) free. No interferance anything.
	 *
	 * Just know that if it grows until around 0x8000 (where the SMP startup
	 * code should be situated), we will be in trouble. However, that shouldn't
	 * be a thing.
	 */
	info->KernelStack = 0x800000;

	/* Initialize the GDT and the TSS */
	LoadGDT(info->KernelStack);
	PRINTK::PrintK("GDT Loaded.\r\n");

	/* Jumpstart interrupts */
	PRINTK::PrintK("Loading x86_64 IDT\r\n");
	IDTInit();
	PRINTK::PrintK("IDT Loaded.\r\n");
}

void Init() {
	/* x86 CPU initialization */
	PRINTK::PrintK("Setting up CPU features\r\n");
	x86CPU *defaultCPU = new x86CPU; /* Allocated by BOOTMEM, do not free */

	/* We get the CPU vendor */
	char vendor[13];
	defaultCPU->GetVendor(vendor);
	PRINTK::PrintK("CPU Vendor: %s\r\n", vendor);

	InitDevices();
}
}
