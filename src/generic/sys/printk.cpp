#include <mm/string.hpp>
#include <mm/memory.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <dev/earlycon.hpp>


static PRINTK::LogMessage LastMessage;
static PRINTK::Loglevel GlobalLoglevel;

namespace PRINTK {
static void FlushMessage() {
	KInfo *info = GetInfo();

	if(GlobalLoglevel >= LastMessage.Level) {
		if(info->KernelPort != NULL) {
			info->KernelPort->PutString(info->KernelPort, LastMessage.Message);
		}
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
                                        Itoa(buf, 'd', va_arg(ap, i64));
                                        PRINTK::PutStr(buf);
                                        break;
                                case 'x':
                                        Itoa(buf, 'x', va_arg(ap, i64));
                                        PRINTK::PutStr(buf);
                                        break;
                                case '%':
                                        PRINTK::PutChar('%');
                                        break;
                                case 'c':
                                        PRINTK::PutChar((va_arg(ap, i32)));
                                        break;

                        }
                } else {
                        PRINTK::PutChar(*ptr++);
                }
        }
}

void EarlyInit() {
	LastMessage.Level = PRINTK::DEBUG;
	GlobalLoglevel = PRINTK::DEBUG;

	PrintK(DEBUG, MODULE_NAME, "Serial PrintK started.\n");
}
}

