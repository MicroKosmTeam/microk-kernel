#include <string.hpp>
#include <memory.hpp>
#include <printk.hpp>
#include <kinfo.hpp>

#if defined(__x86_64__)
#include <arch/x86/io.hpp>
#endif

namespace PRINTK {
static void PutChar(char ch) {
#if defined(__x86_64__)
	x86::OutB(0xE9, ch);
#endif
}

static void PutStr(char *str) {
	while(*str) PutChar(*str++);
}

void PrintK(char *format, ...) {
        va_list ap;
        va_start(ap, format);

	if (*format++ == PRINTK_ASCII_SOH) {
		switch(*format++) {
			case '0':
				break;
			case '1':
				break;
			case '2':
				break;
			case '3':
				break;
			case '5':
				break;
			case '6':
				break;
			case 'c':
				break;
			case '\0':
				break;
			default:
				va_end(ap);
				return;
		}

		VPrintK(format, ap);
	}

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
                                        Itoa(buf, 'd', va_arg(ap, i64));
                                        PutStr(buf);
                                        break;
                                case 'x':
                                        Itoa(buf, 'x', va_arg(ap, i64));
                                        PutStr(buf);
                                        break;
                                case '%':
                                        PutChar('%');
                                        break;
                                case 'c':
                                        PutChar((va_arg(ap, i32)));
                                        break;

                        }
                } else {
                        PutChar(*ptr++);
                }
        }
}
}

