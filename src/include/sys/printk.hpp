#pragma once
#include <cdefs.h>
#include <cstdint.hpp>
#include <sys/timer.hpp>

#include <stdarg.h>

#define PRINTK_SOH       "\001"
#define PRINTK_ASCII_SOH '\001'

#define PRINTK_CONTINUE PRINTK_SOH "c"

#define PRINTK_FATAL    PRINTK_SOH "0"
#define PRINTK_CRITICAL PRINTK_SOH "1"
#define PRINTK_ERROR    PRINTK_SOH "2"
#define PRINTK_WARNING  PRINTK_SOH "3"
#define PRINTK_NORMAL   PRINTK_SOH "4"
#define PRINTK_EXTRA    PRINTK_SOH "5"
#define PRINTK_DEBUG    PRINTK_SOH "6"

namespace PRINTK {
	struct LogMessage {
		u8 StartOfHeader;
		u8 Loglevel;
		utime Timestamp;

		u8 Message[1];
	}__attribute__((packed));

	void PrintK(char *format, ...);
	void VPrintK(char *format, va_list ap);

	void EarlyInit();
}
