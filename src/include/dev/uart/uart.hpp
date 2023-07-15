#pragma once
#include <cdefs.h>
#ifdef CONFIG_HW_UART
#include <stdint.h>
#include <stddef.h>
#include <dev/dev.hpp>

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

class UARTDevice : public Device {
public:
	UARTDevice() { active = false; }

	uint64_t Ioctl(uint64_t request, va_list ap);

	#if defined(ARCH_x64)
	uint64_t Init(SerialPorts serialPort);
	#elif defined(ARCH_aarch64)
	uint64_t Init(uintptr_t address);
	#endif
	void PutStr(const char* str);
	void PutChar(const char ch);
	int GetChar();
private:
	uintptr_t port;

	bool active;

	int isTransmitEmpty();
	int serialReceived();
};

#endif
