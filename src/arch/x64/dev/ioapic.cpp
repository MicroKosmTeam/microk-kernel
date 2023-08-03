#include <arch/x64/dev/ioapic.hpp>

namespace x86_64 {
IOAPIC::IOAPIC(uintptr_t address) {
	Address = address;
}

uintmax_t IOAPIC::Ioctl(uintmax_t request, ...) {
	va_list ap;
	va_start(ap, request);

	uintmax_t result;

	switch (request) {
		case IOAPIC_IOCTL_REQUEST_READ_IOAPIC:
			result = ReadIOAPIC(va_arg(ap, uint32_t));
			break;
		case IOAPIC_IOCTL_REQUEST_WRITE_IOAPIC:
			WriteIOAPIC(va_arg(ap, uint32_t), va_arg(ap, uint32_t));
			result = 0;
			break;
		default:
			result = 0;
	}

	va_end(ap);

	return result;
}

uint32_t IOAPIC::ReadIOAPIC(uint32_t reg) {
	volatile uint32_t *ioapic = (volatile uint32_t*)Address;
	
	ioapic[0] = (reg & 0xFF); /* Write to address select */
	return ioapic[4]; /* Return selected register */
}

void IOAPIC::WriteIOAPIC(uint32_t reg, uint32_t value) {
	volatile uint32_t *ioapic = (volatile uint32_t*)Address;
	
	ioapic[0] = (reg & 0xFF); /* Write to address select */
	ioapic[4] = value; /* Write to selected register */
}
}
