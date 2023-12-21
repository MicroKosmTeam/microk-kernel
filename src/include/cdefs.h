#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__x86_64__)
#elif defined(__aarch64__)
#else
#error Unsupported architecture
#endif

#define PAGE_PROTECTION_READ    0b0001
#define PAGE_PROTECTION_WRITE   0b0010
#define PAGE_PROTECTION_EXEC    0b0100
#define PAGE_PROTECTION_NOCACHE 0b1000

#define EAGAIN      1
#define EBADREQUEST  2
#define EBUSY       3
#define EFAULT      4
#define EINVALID    5
#define ENOSECTION  6
#define ENODRIVER   7
#define ENODEVICE   8
#define ENOMEMORY   9
#define EPERMISSION 10
#define ENOTPRESENT 11
