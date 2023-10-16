#include <dev/earlycon.hpp>
#ifdef CONFIG_HW_UART
#include <stdarg.h>

#include <sys/io.hpp>

namespace DEV {
namespace EARLYCON {

static inline void UARTOut(DeviceMemory *port, size_t offset, uint8_t data) {
	switch(port->Type) {
		case MEMORY_SYSGENERAL:
			break;
		case MEMORY_SYSIO:
			OutB(port->Address + offset, data);
			break;
	}
}

static inline uint8_t UARTIn(DeviceMemory *port, size_t offset) {
	switch(port->Type) {
		case MEMORY_SYSGENERAL:
			return '\0';
		case MEMORY_SYSIO:
			return InB(port->Address + offset);
	}

	return '\0';
}

static inline int IsTransmitEmpty(UARTDevice *device) {
#if defined(ARCH_x64)
	return UARTIn(&device->Port, 5) & 0x20;
#else
	return 0;
#endif
}

static inline int SerialReceived(UARTDevice *device) {
#if defined(ARCH_x64)
	return UARTIn(&device->Port, 5) & 1;
#else
	return 0;
#endif
}

Device *CreateUARTDevice() {
	UARTDevice *device = new DEV::EARLYCON::UARTDevice;

	device->Initialize = InitializeDevice;
	device->Deinitialize = NULL;
	device->Ioctl = NULL;

	return (Device*)device;
}

int InitializeDevice(Device *device, va_list ap) {
	UARTDevice *uartDevice = (UARTDevice*)device;

	uintptr_t port = va_arg(ap, uintptr_t);
	int type = va_arg(ap, int);

	uartDevice->Timeout = -1;

	uartDevice->Port.Address = port;
	uartDevice->Port.Type = type;

#if defined(ARCH_x64)
        UARTOut(&uartDevice->Port, 1, 0x00);    // Disable all interrupts
        UARTOut(&uartDevice->Port, 3, 0x80);    // Enable DLAB (set baud rate divisor)
        UARTOut(&uartDevice->Port, 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
        UARTOut(&uartDevice->Port, 1, 0x00);    //                  (hi byte)
        UARTOut(&uartDevice->Port, 3, 0x03);    // 8 bits, no parity, one stop bit
        UARTOut(&uartDevice->Port, 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
        UARTOut(&uartDevice->Port, 4, 0x0B);    // IRQs enabled, RTS/DSR set
        UARTOut(&uartDevice->Port, 4, 0x1E);    // Set in loopback mode, test the serial chip
        UARTOut(&uartDevice->Port, 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

        // Check if serial is faulty (i.e: not same byte as sent)
        if (UARTIn(&uartDevice->Port, 0) != 0xAE) {
		uartDevice->Active = false;
                return -EFAULT;
        }
	
	uartDevice->Active = true;

        // If serial is not faulty set it in normal operation mode
        // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
        UARTOut(&uartDevice->Port, 4, 0x0F);
#else
	uartDevice->Active = false
#endif
	
	return 0;
}

int PutString(UARTDevice *device, const char* str) {
	if (device == NULL || !device->Active) return -EINVALID;

	char *character = (char*)str;
	while(*character) {
                if(int ret = PutChar(device, *character++)) {
			return ret;
		}
        }

	return 0;
}

int PutChar(UARTDevice *device, const char ch) {
	if (device == NULL || !device->Active) return -EINVALID;

        for(size_t time = 0; time < device->Timeout; ++time) {
		if(IsTransmitEmpty(device)) {
        		UARTOut(&device->Port, 0, ch);
			return 0;
		}
	}

	return -EBUSY;

}

int GetChar(UARTDevice *device) {
	if (device == NULL || !device->Active) return -EINVALID;
        
	for(size_t time = 0; time < device->Timeout; ++time) {
		if(SerialReceived(device)) {
			return UARTIn(&device->Port, 0);
		}
	}

	return -EAGAIN;
}


}
}






#endif
