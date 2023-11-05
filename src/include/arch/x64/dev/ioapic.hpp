#pragma once
#include <cstdint.hpp>

#include <dev/dev.hpp>

#define IOAPIC_IOCTL_REQUEST_READ_IOAPIC 0x01
#define IOAPIC_IOCTL_REQUEST_WRITE_IOAPIC 0x02

#define IOAPIC_REGISTER_IOAPICID  0x00
#define IOAPIC_REGISTER_IOAPICVER 0x01
#define IOAPIC_REGISTER_IOAPICARB 0x02

#define IOAPIR_REGISTER_IOAPICREDTBL(n) (0x10 + 2 * n)

namespace x86_64::IOAPIC {
	struct RedirectionEntry {
		u8 Vector;
		u8 DeliveryMode : 3;
		u8 DestinationMode : 1; /* 0 is physical, 1 is logical */
		u8 DeliveryStatus : 1;
		u8 PinPolarity : 1;
		u8 RemoteIRR : 1;
		u8 TriggerMode : 1;
		u8 Mask : 1;
		u8 Reserved0[4];
		u8 Reserved1 : 7;
		u8 Destination;
	}__attribute__((packed));

	struct IOAPIC : public DEV::Device {
		u32 ID;
		u32 GlobalSystemInterruptBase;

		uptr Base;
		uptr MappedAddress;

		u8 Version;
		u8 MaxRedirectionEntry;
	};
	

	DEV::Device *CreateIOAPICDevice();

	int InitializeDevice(DEV::Device *device, va_list ap);
	int DeinitializeDevice(DEV::Device *device, va_list ap);
	intmax_t Ioctl(DEV::Device *device, request_t request, va_list ap);

	int ReadIOAPIC(IOAPIC *device, usize registerSelector, u32 *value);
	int WriteIOAPIC(IOAPIC *device, usize registerSelector, u32 value);
	int ReadIOAPICRedirectionEntry(IOAPIC *device, u8 entryNumber, RedirectionEntry *entry);
	int WriteIOAPICRedirectionEntry(IOAPIC *device, u8 entryNumber, RedirectionEntry *entry);
}
