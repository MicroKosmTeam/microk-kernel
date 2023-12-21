#pragma once
#include <dev/uart.hpp>

namespace DEV::UART16650 {
	Device *CreateUARTDevice();

	int InitializeDevice(Device *device, va_list ap);
	int DeinitializeDevice(Device *device, va_list ap);
	intmax_t Ioctl(Device *device, request_t request, va_list ap);

	int PutChar(UART::UARTDevice *device, const char ch);
	int PutString(UART::UARTDevice *device, const char *str);

	int GetChar(UART::UARTDevice *device);

}
