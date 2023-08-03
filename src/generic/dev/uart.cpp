#include <dev/uart.hpp>
#ifdef CONFIG_HW_UART
#include <stdarg.h>

#if defined(ARCH_x64)
#include <arch/x64/io/io.hpp>

inline void OutB(uint16_t port, uint8_t value) {
	return x86_64::OutB(port, value);
}

inline uint8_t InB(uint16_t port) {
	return x86_64::InB(port);
}

#elif defined(ARCH_aarch64)

inline void OutB(uintptr_t port, uint8_t value) {
	volatile uint8_t *data = (volatile uint8_t*)port;
	*data = value;
}

inline uint8_t InB(uintptr_t port) {
	return 0;
}



#endif

uintmax_t UARTDevice::Ioctl(uintmax_t request, ...) {
	va_list ap;
	va_start(ap, request);

	uintmax_t result;

	switch (request) {
		case 0: // Init();
			result = Init(va_arg(ap, int));
			break;
		case 1: // PutChar();
			PutChar((const char)va_arg(ap, int));
			result = 0;
			break;
		case 2: // PutStr();
			PutStr((const char*)va_arg(ap, int*));
			result = 0;
			break;
		case 3: // GetChar();
			result = GetChar();
			break;
		default:
			result = 0;
	}

	va_end(ap);

	return result;
}

#if defined(ARCH_x64)
uint64_t UARTDevice::Init(SerialPorts serialPort) {
	port = (uintptr_t)serialPort;

        OutB(port + 1, 0x00);    // Disable all interrupts
        OutB(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
        OutB(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
        OutB(port + 1, 0x00);    //                  (hi byte)
        OutB(port + 3, 0x03);    // 8 bits, no parity, one stop bit
        OutB(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
        OutB(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
        OutB(port + 4, 0x1E);    // Set in loopback mode, test the serial chip
        OutB(port + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

        // Check if serial is faulty (i.e: not same byte as sent)
        if(InB(port + 0) != 0xAE) {
		active = false;
                return 1;
        }

        // If serial is not faulty set it in normal operation mode
        // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
        OutB(port + 4, 0x0F);

	active = true;
        return 0;
}

#elif defined(ARCH_aarch64)
uint64_t UARTDevice::Init(uintptr_t address) {
	port = address;
	active = true;
}
#endif


void UARTDevice::PutStr(const char* str) {
	if (!active) return;
        for (size_t i = 0; 1; i++) {
                char character = (uint8_t) str[i];

                if (character == '\0') {
                        return;
                }

                PutChar(character);
        }
}

void UARTDevice::PutChar(const char ch) {
	if (!active) return;
        while (isTransmitEmpty() == 0);

        OutB(port, ch);
}

int UARTDevice::GetChar() {
	if(!active) return 0;
	while (serialReceived() == 0);

	return InB(port);
}

int UARTDevice::isTransmitEmpty() {
#if defined(ARCH_x64)
	return InB(port + 5) & 0x20;
#else
	return 1;
#endif
}

int UARTDevice::serialReceived() {
#if defined(ARCH_x64)
	return InB(port + 5) & 1;
#else
	return 0;
#endif
}
#endif
