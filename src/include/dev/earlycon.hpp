#pragma once
#include <cdefs.h>
#include <stdint.h>
#include <stddef.h>
#include <dev/dev.hpp>

namespace DEV {
	namespace EARLYCON {

#if defined(ARCH_x64)
		enum SerialPorts {
			COM1 = 0x3f8,
			COM2 = 0x2f8,
			COM3 = 0x3e8,
			COM4 = 0x2e8,
			COM5 = 0x5f8,
			COM6 = 0x4f8,
			COM7 = 0x5e8,
			COM8 = 0x4e8
		};
#endif

		struct UARTDevice : public Device {
			bool Active;
			DeviceMemory Port;

			size_t Timeout;
		};

		Device *CreateUARTDevice();

		int InitializeDevice(Device *device, va_list ap);
		int DeinitializeDevice(Device *device, va_list ap);
		intmax_t Ioctl(Device *device, request_t request, va_list ap);

		int PutChar(UARTDevice *device, const char ch);
		int PutString(UARTDevice *device, const char *str);

		int GetChar(UARTDevice *device);
	}
}
