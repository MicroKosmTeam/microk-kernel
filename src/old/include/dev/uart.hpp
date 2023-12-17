#pragma once
#include <dev/dev.hpp>

namespace DEV::UART {
	struct UARTDevice : public Device {
		bool Active;
		DeviceMemory Port;

		usize Timeout;
	
		int (*PutChar)(UARTDevice *device, const char ch);
		int (*PutString)(UARTDevice *device, const char *str);

		int (*GetChar)(UARTDevice *device);
	};

	Device *CreateUARTDevice(const char *type);
}
