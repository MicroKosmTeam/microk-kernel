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

__attribute__((__aligned__((0x10)))) volatile char InterruptStack[64 * 1024];

namespace x86_64 {
/* Function that initializes platform-specific features */
void Init() {
	KInfo *info = GetInfo();

	/* We first of all get the position of the kernel stack and save it
	   as we will use it to initialize the TSS */
	info->kernelStack = &InterruptStack[64 * 1024 - 1];

	/* Initialize the GDT and the TSS */
	LoadGDT(info->kernelStack);
	PRINTK::PrintK("GDT Loaded.\r\n");

	/* Jumpstart interrupts */
	PRINTK::PrintK("Loading x86_64 IDT\r\n");
	IDTInit();
	PRINTK::PrintK("IDT Loaded.\r\n");

	asm volatile ("push %rax\n\t"
			"push %rbx\n\t"
			"pushfq\n\t"
			"pop %rax\n\t"
			"mov $0xFFFFFFFFFFFFCFFF, %rbx\n\t"
			"and %rbx, %rax\n\t"
			"push %rax\n\t"
			"popfq\n\t"
			"pop %rbx\n\t"
			"pop %rax");

	/* x86 CPU initialization */
	x86CPU *defaultCPU = new x86CPU; /* Allocated by BOOTMEM, do not free */

	/* We get the CPU vendor */
	char vendor[13];
	defaultCPU->GetVendor(vendor);
	PRINTK::PrintK("CPU Vendor: %s\r\n", vendor);
}
}
