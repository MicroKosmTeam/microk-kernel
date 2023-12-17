#pragma once
#include <cstdint.hpp>

#include <stdarg.h>
#include <cdefs.h>

typedef intmax_t request_t;

namespace DEV {
	struct DeviceMemory {
		uptr Address;
		int Type;
	};

	struct Device {
		int (*Initialize)(Device *device, va_list ap);
		int (*Deinitialize)(Device *device, va_list ap);

		intmax_t (*Ioctl)(Device *device, request_t request, va_list ap);
	};

	int InitializeDevice(Device *device, ...);
	int DeinitializeDevice(Device *device, ...);
	intmax_t Ioctl(Device *device, request_t request, ...);
}
