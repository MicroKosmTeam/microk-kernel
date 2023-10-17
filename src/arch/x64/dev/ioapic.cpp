#include <arch/x64/dev/ioapic.hpp>

namespace x86_64 {
DEV::Device *CreateIOAPICDevice() {
	IOAPIC *ioapic = new IOAPIC;
	
	return (DEV::Device*)ioapic;
}

int InitializeDevice(DEV::Device *device, va_list ap) {
	(void)device;
	(void)ap;

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
