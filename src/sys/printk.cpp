#include <cdefs.h>
#include <stdarg.h>
#include <stdint.h>
#include <mm/string.hpp>
#include <sys/mutex.hpp>
#include <mm/bootmem.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>

#ifdef CONFIG_HW_UART
UARTDevice *kernelPort;
#endif

static SpinLock PrintkSpinlock;

namespace PRINTK {
void PutChar(char ch) {
#ifdef CONFIG_HW_UART
	kernelPort->PutChar(ch);
#endif
}

void PutStr(char *str) {
#ifdef CONFIG_HW_UART
	kernelPort->PutStr(str);
#endif
}

void PrintK(char *format, ...) {
	PrintkSpinlock.Lock();
        va_list ap;
        va_start(ap, format);

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

        va_end(ap);
	PrintkSpinlock.Unlock();
}

void EarlyInit() {
	KInfo *info = GetInfo();

#ifdef CONFIG_HW_UART
	info->kernelPort = (UARTDevice*)BOOTMEM::Malloc(sizeof(UARTDevice) + 1);
	info->kernelPort->Init(COM1);
	info->kernelPort->PutStr("Serial PrintK started.\n");
	kernelPort = info->kernelPort;
#endif
}
}

