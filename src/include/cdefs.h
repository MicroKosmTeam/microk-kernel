#pragma once
#include <stdint.h>
#include <stddef.h>
#include <cstdint.h>

#if defined(__x86_64__)
#elif defined(__aarch64__)
#else
#error "Unknown architecture"
#endif

#define HALT for(;;)

#define USER_MANAGER_NAME "manager.kmd"

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
#define INIT_INITIAL_STACK_SIZE (64 * 1024)
#define INIT_MAXIMUM_STACK_GROWTH (2 * 1024 * 1024)

#define VIRTUAL_REGISTERS_SIZE 1024

#define SCHEDULER_PRIORITIES   64
#define SCHEDULER_MIN_PRIORITY ((u8)SCHEDULER_PRIORITIES - 1)
#define SCHEDULER_MAX_PRIORITY ((u8)0x00)

typedef enum {
	SYSCALL_VECTOR_DEBUG = 0,
	SYSCALL_VECTOR_CAPCTL,
	SYSCALL_VECTOR_ARCHCTL,
	SYSCALL_VECTOR_TASKCTL,
	SYSCALL_VECTOR_YEILD,
} SYSCALL_VECTOR;

typedef enum {
	SYSCALL_CAPCTL_GET_PTR = 1,
	SYSCALL_CAPCTL_GET_NEXT_PTR,
	SYSCALL_CAPCTL_GET_NEXT_SLOT,
	SYSCALL_CAPCTL_GET_UT,
	SYSCALL_CAPCTL_GET_OBJ,
	SYSCALL_CAPCTL_ADD_CNODE,
	SYSCALL_CAPCTL_SPLIT,
	SYSCALL_CAPCTL_RETYPE,
	SYSCALL_CAPCTL_DEBUG,
	SYSCALL_CAPCTL_MAP_INTERMEDIATE,
	SYSCALL_CAPCTL_MAP_PAGE,
	SYSCALL_CAPCTL_REPLYRECV,
	SYSCALL_CAPCTL_CALL,
} SYSCALL_CAPCTL_OPS;

typedef enum {

} SYSCALL_ARCHCTL_OPS;

typedef enum {
	SYSCALL_TASKCTL_REGS_READ = 1,
	SYSCALL_TASKCTL_REGS_WRITE,
} SYSCALL_TASKCTL_OPS;

typedef enum {
	NULL_SLOT = 0,
	CSPACE_SLOT,
	ROOT_CNODE_SLOT,
	TASK_CONTROL_BLOCK_SLOT,
	VIRTUAL_SPACE_ROOT_SLOT,
	MEMORY_MAP_CNODE_SLOT,
	SLOT_COUNT,
} ROOT_CNODE_SLOTS;

/* All the available object types that can be
 * mapped to a capability.
 *
 * 00h - NULL_CAPABILITY:
 * 01h - UNTYPED:
 * 02h - FRAMES:
 * 03h - CSPACE:
 * 04h - CNODE:
 * 05h - DOMAIN:
 * 06h - SCHEDULER:
 * 07h - TASK_CONTROL_BLOCK:
 * 08h - SCHEDULER_CONTEXT:
 * 09h - PAGING_STRUCTURE:
 * 0Ah - OBJECT_TYPE_COUNT:
 *  Constant that keeps count of the total amount
 *  of allowed object types.
 * FFh - RESERVED_SLOT:
 *  Statically assigned slots that haven't yet been filled.
 */
typedef enum {
	UNTYPED = 0,
	FRAMES,
	VIRTUAL_MEMORY_MAPPING,
	VIRTUAL_MEMORY_PAGING_STRUCTURE,
	CSPACE,
	CNODE,
	CPU_DOMAIN,
	PROCESS_SCHEDULER,
	TASK_CONTROL_BLOCK,
	TASK_SCHEDULER_CONTEXT,
	OBJECT_TYPE_COUNT,
	NULL_CAPABILITY,
	RESERVED_SLOT
} OBJECT_TYPE;

/* The rights that can be given to a capability
 *
 *      0 - NONE:
 *       Nothing can be done with the capability.
 *
 * 1 << 0 - ACCESS:
 *       The capability can used by getting its pointer.
 *
 * 1 << 2 - RETYPE:
 *       The object can be retyped to another kind of object.
 *
 * 1 << 3 - GRANT:
 *       The ownership of the capability can be given to some other CSpace
 *       (not reversible).
 *
 * 1 << 4 - MINT:
 * 	 Can create a new capability can be forged with the same or fewer
 * 	 priviledges and then granted to someone else.
 *
 * 1 << 5 - REVOKE:
 *       The capability can be revoked and it will become UNTYPED.
 *
 *  -- FOR FRAMES ONLY --
 * 1 << 6 - READ:
 *       The frame can be mapped in the virtual space.
 *
 * 1 << 7 - WRITE:
 *       The frame can be mapped in the virtual space and written to.
 *
 * 1 << 8 - EXECUTE:
 *       The frame can be mapped in the virtual space and have NX
 *       protection disabled on it.
 */
typedef enum {
	NONE = 0,
	ACCESS = 1 << 0,
	SPLIT = 1 << 1,
	RETYPE = 1 << 2,
	GRANT = 1 << 3,
	MINT = 1 << 4,
	REVOKE = 1 << 5,
	READ = 1 << 6,
	WRITE = 1 << 7,
	EXECUTE = 1 << 8,
} CAPABILITY_RIGHTS;


