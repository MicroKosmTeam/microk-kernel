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
	VMM::MapPage(info->KernelVirtualSpace, ioapic->Base, ioapic->MappedAddress, VMM_FLAGS_KERNEL_DEVICE);

	ioapic->GlobalSystemInterruptBase = va_arg(ap, u32);

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "IOAPIC 0x%x at 0x%x\r\n", ioapic->ID, ioapic->Base);

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
}
