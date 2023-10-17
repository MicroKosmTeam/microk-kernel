#pragma once
#include <cdefs.h>
#include <cstdint.hpp>

#include <stdarg.h>
#include <sys/time.hpp>

namespace PRINTK {
	enum Loglevel {
		CRITICAL = 0,
		ERROR,
		WARNING,
		ISSUE,
		INFO,
		DEBUG,
	};

	struct LogMessage {
		LogMessage *Next;

		Loglevel Level;
		time_t Timestamp;
		usize Length;

		char Module[MAX_KERNEL_MODULE_NAME_LENGTH];
		char Message[MAX_PRINTK_MESSAGE_LENGTH];
	};

	void PrintK(Loglevel loglevel, const char *moduleName, char *format, ...);
	void VPrintK(char *format, va_list ap);

	void EarlyInit();
}
