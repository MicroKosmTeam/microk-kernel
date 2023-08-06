#include <cdefs.h>
#include <stdint.h>
#include <mm/string.hpp>
#include <sys/mutex.hpp>
#include <sys/queue.hpp>
#include <mm/bootmem.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>

#ifdef CONFIG_HW_UART
UARTDevice *kernelPort;
#endif

namespace PRINTK {

#define TERMINAL_SIZE 240

static SpinLock PrintKSpinlock;
static char TerminalColumn[TERMINAL_SIZE + 1];
static size_t TerminalPosition;

void FlushBuffer() {
#ifdef CONFIG_HW_UART
	kernelPort->PutStr(TerminalColumn);
#endif
	memset(TerminalColumn, 0, TERMINAL_SIZE + 1);
	TerminalPosition = 0;
}

void PutChar(char ch) {

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
	
}

void PutStr(char *str) {
	while(*str) PutChar(*str++);
}

void PrintK(char *format, ...) {
        va_list ap;
        va_start(ap, format);

	PrintKSpinlock.Lock();
	VPrintK(format, ap);
	PrintKSpinlock.Unlock();

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
	info->kernelPort = (UARTDevice*)BOOTMEM::Malloc(sizeof(UARTDevice) + 1);
#if defined(ARCH_x64)
	info->kernelPort->Init(COM1);
#elif defined(ARCH_aarch64)
	info->kernelPort->Init(0x09000000);
#endif
	kernelPort = info->kernelPort;

	PrintK("Serial PrintK started.\n");
#endif
}
}

