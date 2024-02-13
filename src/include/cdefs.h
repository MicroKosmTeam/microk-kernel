#pragma once
#include <stdint.h>
#include <stddef.h>
#include <cstdint.h>

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

typedef enum {
	ENOERR = 0,
	EAGAIN,
	EBADREQUEST,
	EBUSY,
	EFAULT,
	EINVALID,
	ENOCAP,
	ENOMEMORY,
	EPERMISSION,
	ENOTPRESENT,
} ERROR_VECTOR;

/* Limiting maximum bootmem allocation space */
#define MAXIMUM_BOOTMEM_MEMORY (64 * 1024)

#define SCHEDULER_PRIORITIES   64
#define SCHEDULER_MIN_PRIORITY ((u8)SCHEDULER_PRIORITIES - 1)
#define SCHEDULER_MAX_PRIORITY ((u8)0x00)

typedef enum {
	SYSCALL_VECTOR_DEBUG = 0,
	SYSCALL_VECTOR_YEILD,
	SYSCALL_VECTOR_CAPCTL,
	SYSCALL_VECTOR_ARCHCTL,
	SYSCALL_VECTOR_CALL,
	SYSCALL_VECTOR_REPLYRECV,
} SYSCALL_VECTOR;

typedef enum {
	SYSCALL_CAPCTL_GET_PTR = 1,
	SYSCALL_CAPCTL_GET_CAP,
	SYSCALL_CAPCTL_RETYPE,
} SYSCALL_CAPCTL_OPS;

typedef enum {
	SYSCALL_ARCHCTL_MAP_INTERMEDIATE = 1,
	SYSCALL_ARCHCTL_MAP_PAGE,
} SYSCALL_ARCHCTL_OPS;
