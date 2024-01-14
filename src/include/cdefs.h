#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__x86_64__)
#elif defined(__aarch64__)
#else
#error Unsupported architecture
#endif

#define HALT for(;;)

#define PAGE_PROTECTION_READ    ((usize)(1 << 0))
#define PAGE_PROTECTION_WRITE   ((usize)(1 << 1))
#define PAGE_PROTECTION_EXEC    ((usize)(1 << 2))
#define PAGE_PROTECTION_NOCACHE ((usize)(1 << 3)) 
#define PAGE_PROTECTION_HUGE    ((usize)(1 << 4))
#define PAGE_PROTECTION_HUGER   ((usize)(1 << 5))

#define EAGAIN      1
#define EBADREQUEST 2
#define EBUSY       3
#define EFAULT      4
#define EINVALID    5
#define ENOSECTION  6
#define ENODRIVER   7
#define ENODEVICE   8
#define ENOMEMORY   9
#define EPERMISSION 10
#define ENOTPRESENT 11

/* Limiting maximum bootmem allocation space */
#define MAXIMUM_BOOTMEM_MEMORY (64 * 1024)

#define SCHEDULER_PRIORITIES   256
#define SCHEDULER_MIN_PRIORITY ((u8)0xff)
#define SCHEDULER_MAX_PRIORITY ((u8)0x00)

#define SYSCALL_VECTOR_DEBUG     0
#define SYSCALL_VECTOR_YEILD     1
#define SYSCALL_VECTOR_CALL      2
#define SYSCALL_VECTOR_REPLYRECV 3
