#include <dev/earlycon.hpp>
#ifdef CONFIG_HW_UART
#include <stdarg.h>
#include <mm/string.hpp>
#include <sys/printk.hpp>

namespace DEV::EARLYCON {
Device *InitializeEarlycon(char *arguments) {
	char *endChar;
	char *type = Strtok(arguments, ", ", &endChar);
	char *memtype = Strtok(NULL, ", ", &endChar);
	char *addr = Strtok(NULL, ", ", &endChar);
	
	PRINTK::PrintK(PRINTK_DEBUG 
		       "Creating earlycon:\r\n"
		       " - Name: %s\r\n"
		       " - Memtype: %s\r\n"
		       " - Addr: %s\r\n",
		       type, memtype, addr);

	UART::UARTDevice *device = (UART::UARTDevice*)UART::CreateUARTDevice(type);
	DEV::InitializeDevice(device, Atoi(addr), Atoi(memtype));

	return (Device*)device;
}
}

#endif
