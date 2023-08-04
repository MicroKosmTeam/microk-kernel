#pragma once
#include <autoconf.h>

#if defined(__x86_64__)
#define ARCH_x64
#elif defined(__aarch64__)
#define ARCH_aarch64
#else
#error Unsupported architecture
#endif

#define HIGHER_HALF 0xFFFF800000000000

#define SYSCALL_DEBUG_PRINTK      0x0001

#define SYSCALL_MEMORY_GETINFO    0x1000
#define SYSCALL_MEMORY_VMALLOC    0x1001
#define SYSCALL_MEMORY_VMFREE     0x1002
#define SYSCALL_MEMORY_MMAP       0x1003
#define SYSCALL_MEMORY_UNMAP      0x1004
#define SYSCALL_MEMORY_INOUT      0x1005

#define SYSCALL_PROC_EXEC         0x2000
#define SYSCALL_PROC_FORK         0x2001
#define SYSCALL_PROC_RETURN       0x2002
#define SYSCALL_PROC_EXIT         0x2003
#define SYSCALL_PROC_WAIT         0x2004
#define SYSCALL_PROC_KILL         0x2005

#define SYSCALL_MODULE_REGISTER          0x3000
#define SYSCALL_MODULE_UNREGISTER        0x3001
#define SYSCALL_MODULE_BUFFER_CREATE     0x3100
#define SYSCALL_MODULE_BUFFER_MAP        0x3101
#define SYSCALL_MODULE_BUFFER_UNMAP      0x3102
#define SYSCALL_MODULE_BUFFER_DELETE     0x3103
#define SYSCALL_MODULE_MESSAGE_HANDLER   0x3200
#define SYSCALL_MODULE_MESSAGE_SEND      0x3201
#define SYSCALL_MODULE_SECTION_REGISTER      0x3500
#define SYSCALL_MODULE_SECTION_GET           0x3501
#define SYSCALL_MODULE_SECTION_UNREGISTER    0x3502

#define SYSCALL_FILE_OPEN         0x4000
#define SYSCALL_FILE_READ         0x4001
#define SYSCALL_FILE_WRITE        0x4002
#define SYSCALL_FILE_CLOSE        0x4003

#define SYSCALL_KERN_OVERRIDE     0x5000

#define BUFFER_TYPE_COMMUNICATION_MODULE_KERNEL 0xA000
#define BUFFER_TYPE_LOG_PRINTK                  0xA001
#define BUFFER_TYPE_DATA_MODULE_GENERIC         0xA002
#define BUFFER_TYPE_HW_IO_GENERIC               0xA003

#define BUFFER_OPERATION_READDATA_SYNC          0xB000
#define BUFFER_OPERATION_WRITEDATA_SYNC         0xB001

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
