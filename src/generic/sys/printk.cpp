#include <mm/string.hpp>
#include <mm/memory.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <dev/earlycon.hpp>

#include <arch/x64/io/io.hpp>

static usize MessagePos;
static char Message[MAX_PRINTK_MESSAGE_LENGTH];

namespace PRINTK {
static void FlushMessage() {
	KInfo *info = GetInfo();
	Message[MessagePos] = '\0';

	if(info->KernelPort != NULL) {
		info->KernelPort->PutString(info->KernelPort, Message);
	}

	MessagePos = 0;
	Memset(Message, 0, MAX_PRINTK_MESSAGE_LENGTH);
}

static void PutChar(char ch) {
	x86_64::OutB(0xE9, ch);

	bool justNewline = false;

	if (MessagePos >= MAX_PRINTK_MESSAGE_LENGTH) {
		FlushMessage();
		justNewline = true;
	}

	Message[MessagePos++] = ch;

	if (ch == '\n' && !justNewline) {
		FlushMessage();
		justNewline = true;
	}
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

void EarlyInit() {
	PrintK(PRINTK_DEBUG MODULE_NAME "Serial PrintK started.\n");
}
}

