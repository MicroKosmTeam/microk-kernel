#include <mm/string.hpp>
#include <mm/memory.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>

#ifdef CONFIG_HW_UART
#include <dev/earlycon.hpp>
#endif


#ifdef CONFIG_HW_UART
static DEV::EARLYCON::UARTDevice *KernelPort = NULL;
#endif

static PRINTK::LogMessage LastMessage;
static PRINTK::Loglevel GlobalLoglevel;

namespace PRINTK {
static void FlushMessage() {
	if(GlobalLoglevel >= LastMessage.Level) {
#ifdef CONFIG_HW_UART
		if(KernelPort != NULL) {/*
			DEV::EARLYCON::PutString(KernelPort, LastMessage.Module);
			DEV::EARLYCON::PutString(KernelPort, " -> ");*/
			DEV::EARLYCON::PutString(KernelPort, LastMessage.Message);
		}
#endif
	}

	Memset(&LastMessage, 0, sizeof(LogMessage));
}

static void PutChar(char ch) {
	bool justNewline = false;

	if (LastMessage.Length + 1 > MAX_PRINTK_MESSAGE_LENGTH) {
		FlushMessage();
		justNewline = true;
	}

	LastMessage.Message[LastMessage.Length++] = ch;

	if (ch == '\n' && !justNewline) {
		FlushMessage();
		justNewline = true;
	}
}

static void PutStr(char *str) {
	while(*str) PRINTK::PutChar(*str++);
}

void PrintK(Loglevel loglevel, const char *moduleName, char *format, ...) {
	(void)moduleName;

        va_list ap;
        va_start(ap, format);

	if(LastMessage.Level > loglevel) {
		LastMessage.Level = loglevel;
	}
/*
	if(LastMessage.Module[0] != '\0' && Strncmp(LastMessage.Module, moduleName, MAX_KERNEL_MODULE_NAME_LENGTH) != 0) {
		FlushMessage();
		Memset(LastMessage.Module, 0, MAX_KERNEL_MODULE_NAME_LENGTH);
	}
	Strncpy(LastMessage.Module, moduleName, MAX_KERNEL_MODULE_NAME_LENGTH);
*/
	VPrintK(format, ap);

        va_end(ap);
}
	
void VPrintK(char *format, va_list ap) {
        char *ptr = format;
        char buf[128];

        while(*ptr) {
                if (*ptr == '%') {
                        ptr++;
                        switch (*ptr++) {
                                case 's':
					PRINTK::PutStr(va_arg(ap, char *));
                                        break;
                                case 'd':
                                case 'u':
                                        Itoa(buf, 'd', va_arg(ap, int64_t));
                                        PRINTK::PutStr(buf);
                                        break;
                                case 'x':
                                        Itoa(buf, 'x', va_arg(ap, int64_t));
                                        PRINTK::PutStr(buf);
                                        break;
                                case '%':
                                        PRINTK::PutChar('%');
                                        break;
                                case 'c':
                                        PRINTK::PutChar((va_arg(ap, int32_t)));
                                        break;

                        }
                } else {
                        PRINTK::PutChar(*ptr++);
                }
        }
}

void EarlyInit() {
	KInfo *info = GetInfo();

	LastMessage.Level = PRINTK::DEBUG;
	GlobalLoglevel = PRINTK::DEBUG;

#ifdef CONFIG_HW_UART
	info->KernelPort = (DEV::EARLYCON::UARTDevice*)DEV::EARLYCON::CreateUARTDevice();
#if defined(ARCH_x64)
	DEV::InitializeDevice((DEV::Device*)info->KernelPort, (uintptr_t)DEV::EARLYCON::COM1, MEMORY_SYSIO);
#elif defined(ARCH_aarch64)
	DEV::InitializeDevice((DEV::Device*)info->KernelPort, 0x09000000);
#endif
	KernelPort = info->KernelPort;
#endif

	PrintK(DEBUG, MODULE_NAME, "Serial PrintK started.\n");
}
}

