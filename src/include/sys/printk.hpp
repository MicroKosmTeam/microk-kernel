#pragma once
#include <cdefs.h>
#ifdef CONFIG_PRINTK

namespace PRINTK {
	void PrintK(char *format, ...);
#ifdef CONFIG_HW_SERIAL
	void EarlyInit();
#endif
}
#endif
