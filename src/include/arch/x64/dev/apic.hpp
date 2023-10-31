#pragma once
#include <cstdint.hpp>

#include <sys/time.hpp>
#include <dev/dev.hpp>

namespace x86_64::APIC {
	enum APICRegisters {
		LAPICIDRegister = 0x20,
		LAPICVersionRegister = 0x30
	};

	struct APICTimer : public TIME::Timer {
	};

	struct APIC : public DEV::Device {
		u32 ID;

		uptr Base;
		uptr MappedAddress;

		bool ProcessorIsBSP;

		APICTimer *Timer;
	};


	DEV::Device *CreateAPICDevice();

	int InitializeDevice(DEV::Device *device, va_list ap);
	int DeinitializeDevice(DEV::Device *device, va_list ap);
	intmax_t Ioctl(DEV::Device *device, request_t request, va_list ap);

	int ReadAPIC(APIC *device, APICRegisters registerSelector, u32 *value);
	int WriteAPIC(APIC *device, APICRegisters registerSelector, u32 value);
}
