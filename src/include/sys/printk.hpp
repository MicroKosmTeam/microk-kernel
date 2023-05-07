#pragma once
#include <stdarg.h>

namespace PRINTK {
	void PrintK(char *format, ...);
	void VPrintK(char *format, va_list ap);
	void EarlyInit();
}
