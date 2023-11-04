#pragma once
#include <cstdint.hpp>

#include <dev/dev.hpp>

#define IOAPIC_IOCTL_REQUEST_READ_IOAPIC 0x01
#define IOAPIC_IOCTL_REQUEST_WRITE_IOAPIC 0x02

namespace x86_64::IOAPIC {
	struct IOAPIC : public DEV::Device {
		u32 ID;
		u32 GlobalSystemInterruptBase;

		uptr Base;
		uptr MappedAddress;
	};

	DEV::Device *CreateIOAPICDevice();

	int InitializeDevice(DEV::Device *device, va_list ap);
	int DeinitializeDevice(DEV::Device *device, va_list ap);
	intmax_t Ioctl(DEV::Device *device, request_t request, va_list ap);

	int ReadIOAPIC(IOAPIC *device, usize registerSelector, u32 *value);
	int WriteIOAPIC(IOAPIC *device, usize registerSelector, u32 value);
}
