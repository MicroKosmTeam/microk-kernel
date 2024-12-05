#include <arch/x86/ioapic.hpp>
#include <printk.hpp>
#include <arch/x86/cpu.hpp>
#include <kinfo.hpp>
#include <vmm.hpp>
#include <pmm.hpp>
#include <capability.hpp>

namespace x86 {
void WriteIOAPICRegister(uptr addr, u8 offset, u32 val) {
    /* tell IOREGSEL where we want to write to */
    *(volatile u32*)(addr) = offset;
    /* write the value to IOWIN */
    *(volatile u32*)(addr + 0x10) = val; 
}
 
uint32_t ReadIOAPICRegister(uptr addr, u8 offset) {
    /* tell IOREGSEL where we want to read from */
    *(volatile u32*)(addr) = offset;
    /* return the data from IOWIN */
    return *(volatile u32*)(addr + 0x10);
}

int InitializeIOAPIC(IOAPIC *ioapic, u32 id, uptr address) {
	KInfo *info = GetInfo();

	ioapic->Base = address;
	ioapic->ID = id;
	ioapic->MappedAddress = VMM::PhysicalToVirtual(ioapic->Base);
	VMM::MMap(info->KernelVirtualSpace, ioapic->Base, ioapic->MappedAddress, PAGE_SIZE, VMM_FLAGS_READ | VMM_FLAGS_WRITE | VMM_FLAGS_NOEXEC);
	PMM::CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);
	CAPABILITY::GenerateCapability(info->RootCSpace, MMIO_MEMORY, ioapic->Base, ACCESS | READ | WRITE);

	PRINTK::PrintK(PRINTK_DEBUG "IOAPIC with ID 0x%x at 0x%x\r\n",
		       ioapic->ID,
		       ioapic->Base);

	return 0;
}
}
