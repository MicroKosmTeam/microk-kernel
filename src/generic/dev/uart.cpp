#include <dev/uart.hpp>
#include <mm/string.hpp>

#include <dev/uart16650.hpp>

struct UARTDriver {
	const char *Name;
	DEV::Device *(*CreateUARTDevice)();
};

static const UARTDriver UARTDriverTable[] = {
	{ "uart8250",  NULL },
	{ "uart16650", DEV::UART16650::CreateUARTDevice},
	{ NULL, NULL }
};

namespace DEV::UART {
Device *CreateUARTDevice(const char *type) {
	UARTDriver const *driver = UARTDriverTable;

	while(driver->Name != NULL) {
		if (Strncmp(driver->Name, type, UNLIMITED_STRING_LENGTH) == 0) {
			return driver->CreateUARTDevice();
		}

		++driver;
	}

	return NULL;
}



}
