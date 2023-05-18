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
void PutStr(char *str) {
	asm ("cli");
#ifdef CONFIG_HW_UART
	kernelPort->PutStr(str);
#endif
	asm ("sti");
}

void PutChar(char ch) {
	asm ("cli");
#ifdef CONFIG_HW_UART
	kernelPort->PutChar(ch);
#endif
	asm ("sti");
}

static SpinLock PrintKSpinlock;

void PrintK(char *format, ...) {
	PrintKSpinlock.Lock();

        va_list ap;
        va_start(ap, format);

	VPrintK(format, ap);

        va_end(ap);
	PrintKSpinlock.Unlock();
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

#ifdef CONFIG_HW_UART
	info->kernelPort = (UARTDevice*)BOOTMEM::Malloc(sizeof(UARTDevice) + 1);
	info->kernelPort->Init(COM1);
	info->kernelPort->PutStr("Serial PrintK started.\n");
	kernelPort = info->kernelPort;
#endif
}
}

