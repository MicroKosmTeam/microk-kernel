#include <arch/x64/dev/ioapic.hpp>
#include <stdarg.h>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>

namespace x86_64::IOAPIC {
DEV::Device *CreateIOAPICDevice() {
	IOAPIC *ioapic = new IOAPIC;

	ioapic->Initialize = InitializeDevice;
	ioapic->Deinitialize = DeinitializeDevice;
	ioapic->Ioctl = Ioctl;
	
	return (DEV::Device*)ioapic;
}

int InitializeDevice(DEV::Device *device, va_list ap) {
	KInfo *info = GetInfo();
	IOAPIC *ioapic = (IOAPIC*)device;
	
	ioapic->ID = va_arg(ap, u32);

	ioapic->Base = va_arg(ap, uptr);
	ioapic->MappedAddress = VMM::PhysicalToVirtual(ioapic->Base);
	VMM::MapPage(info->KernelVirtualSpace, ioapic->Base, ioapic->MappedAddress, VMM_FLAGS_KERNEL_DATA);

	u32 ioapicVersionRegister;
	ReadIOAPIC(ioapic, IOAPIC_REGISTER_IOAPICVER, &ioapicVersionRegister);

	ioapic->Version = ioapicVersionRegister & 0xf;
	ioapic->MaxRedirectionEntry = ((ioapicVersionRegister >> 16) & 0xff) + 1;

	ioapic->GlobalSystemInterruptBase = va_arg(ap, u32);

	ReadIOAPIC(ioapic, IOAPIC_REGISTER_IOAPICVER, &ioapicVersionRegister);

	PRINTK::PrintK(PRINTK_DEBUG "IOAPIC 0x%x (version 0x%x) at 0x%x, handled IRQs: 0x%x\r\n", ioapic->ID, ioapic->Base, ioapic->Version, ioapic->MaxRedirectionEntry);

	RedirectionEntry entry;
	for (u8 entryNumber = 0; entryNumber < ioapic->MaxRedirectionEntry; ++entryNumber) {
		ReadIOAPICRedirectionEntry(ioapic, entryNumber, &entry);

		PRINTK::PrintK(PRINTK_DEBUG "IOAPIC entry 0x%x:\r\n"
				" - Vector:           0x%x\r\n"
				" - Delivery Mode:    0x%x\r\n"
				" - Destination Mode: 0x%x\r\n"
				" - Delivery Status:  0x%x\r\n"
				" - Pin Polarity:     0x%x\r\n"
				" - Remote IRR:       0x%x\r\n"
				" - Trigger Mode:     0x%x\r\n"
				" - Mask:             0x%x\r\n"
				" - Destination:      0x%x\r\n"
				, entryNumber, entry.Vector, entry.DeliveryMode, entry.DestinationMode, entry.DeliveryStatus, entry.PinPolarity, entry.RemoteIRR, entry.TriggerMode, entry.Mask, entry.Destination);
	}

	return 0;
}

int DeinitializeDevice(DEV::Device *device, va_list ap) {
	(void)device;
	(void)ap;

	return 0;
}

intmax_t Ioctl(DEV::Device *device, request_t request, va_list ap) {
	(void)device;
	(void)request;
	(void)ap;

	return 0;
}

int ReadIOAPIC(IOAPIC *device, usize registerSelector, u32 *value) {
	volatile u32 *ioapic = (volatile u32*)device->MappedAddress;
	
	ioapic[0] = (registerSelector & 0xFF); /* Write to address select */
	*value = ioapic[4]; /* Return selected register */

	return 0;
}

int WriteIOAPIC(IOAPIC *device, usize registerSelector, u32 value) {
	volatile u32 *ioapic = (volatile u32*)device->MappedAddress;
	
	ioapic[0] = (registerSelector & 0xFF); /* Write to address select */
	ioapic[4] = value; /* Write to selected register */

	return 0;
}

int ReadIOAPICRedirectionEntry(IOAPIC *device, u8 entryNumber, RedirectionEntry *entry) {
	u32 lo, hi;

	if (entryNumber > device->MaxRedirectionEntry) {
		return -ENOTPRESENT;
	}

	ReadIOAPIC(device, IOAPIR_REGISTER_IOAPICREDTBL(entryNumber), &lo);
	ReadIOAPIC(device, IOAPIR_REGISTER_IOAPICREDTBL(entryNumber) + 1, &hi);

	*(u32*)((uptr)entry) = lo;
	*(u32*)((uptr)entry + sizeof(u32)) = hi;

	/* If the destination is physical, we can only address 16 CPUs,
	 * so we remove the upper bits
	 */
	if (entry->DestinationMode == 0) {
		entry->Destination &= 0xf;
	}

	return 0;
}

int WriteIOAPICRedirectionEntry(IOAPIC *device, u8 entryNumber, RedirectionEntry *entry) {
	u32 lo, hi;

	if (entryNumber > device->MaxRedirectionEntry) {
		return -ENOTPRESENT;
	}

	lo = *(u32*)((uptr)entry);
	hi = *(u32*)((uptr)entry + sizeof(u32));

	WriteIOAPIC(device, IOAPIR_REGISTER_IOAPICREDTBL(entryNumber), lo);
	WriteIOAPIC(device, IOAPIR_REGISTER_IOAPICREDTBL(entryNumber) + 1, hi);

	return 0;
}

}
