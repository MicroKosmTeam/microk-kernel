#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <cdefs.h>

typedef intmax_t request_t;

namespace DEV {
	struct Device {
		int (*Initialize)(Device *device);
		int (*Deinitialize)(Device *device);

		intmax_t (*Ioctl)(Device *device, request_t request, va_list ap);
	};

	int InitializeDevice(Device *device);
	int DeinitializeDevice(Device *device);
	intmax_t Ioctl(Device *device, request_t request, ...);
}

class Device {
public:
	virtual ~Device() { };
	virtual uintmax_t Ioctl(uintmax_t request, ...) = 0;
protected:
};
