#include <cdefs.h>
#include <stdint.h>
#include <mm/string.hpp>
#include <sys/mutex.hpp>
#include <mm/bootmem.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>

#ifdef CONFIG_HW_UART
UARTDevice *KernelPort = NULL;
#endif

namespace PRINTK {

#define TERMINAL_SIZE 240

static SpinLock PrintKSpinlock;
static char TerminalColumn[TERMINAL_SIZE + 1];
static size_t TerminalPosition;

void FlushBuffer() {
#ifdef CONFIG_HW_UART
	if(KernelPort != NULL) KernelPort->PutStr(TerminalColumn);
#endif
	memset(TerminalColumn, 0, TERMINAL_SIZE + 1);
	TerminalPosition = 0;
}

void PutChar(char ch) {
//	PrintKSpinlock.Lock();

	bool justNewline = false;

	if (TerminalPosition + 1 > TERMINAL_SIZE) {
		FlushBuffer();
		justNewline = true;
	}

	TerminalColumn[TerminalPosition++] = ch;

	if ((ch == '\n' || ch == '\r') && !justNewline) {
		FlushBuffer();
		justNewline = true;
	}
	
//	PrintKSpinlock.Unlock();	
}

void PutStr(char *str) {
	while(*str) PutChar(*str++);
}

void PrintK(char *format, ...) {
        va_list ap;
        va_start(ap, format);

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
					PutStr(va_arg(ap, char *));
                                        break;
                                case 'd':
                                case 'u':
                                        itoa(buf, 'd', va_arg(ap, int64_t));
                                        PutStr(buf);
                                        break;
                                case 'x':
                                        itoa(buf, 'x', va_arg(ap, int64_t));
                                        PutStr(buf);
                                        break;
                                case '%':
                                        PutChar('%');
                                        break;
                                case 'c':
                                        PutChar((va_arg(ap, int32_t)));
                                        break;

                        }
                } else {
                        PutChar(*ptr++);
                }
        }
}

void EarlyInit() {
	KInfo *info = GetInfo();

	memset(TerminalColumn, 0, TERMINAL_SIZE + 1);

#ifdef CONFIG_HW_UART
	info->KernelPort = (UARTDevice*)BOOTMEM::Malloc(sizeof(UARTDevice));
#if defined(ARCH_x64)
	info->KernelPort->Init(COM1);
#elif defined(ARCH_aarch64)
	info->KernelPort->Init(0x09000000);
#endif
	KernelPort = info->KernelPort;

	PrintK("Serial PrintK started.\n");
#endif
}
}

