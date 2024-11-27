#pragma once
#include <stdint.h>
#include <stddef.h>

enum PT_Flag {
	Present       = 0,
	ReadWrite     = 1,
	UserSuper     = 2,
	WriteThrough  = 3,
	CacheDisabled = 4,
	Accessed      = 5,
	HugePages     = 7,
	Global        = 8,
	Custom0       = 9,
	Custom1       = 10,
	Custom2       = 11,
	NX            = 63 // Only if supported
};

#define VMM_FLAGS_READ       ((size_t)1 << x86_64::PT_Flag::Present)
#define VMM_FLAGS_NOEXEC     ((size_t)1 << x86_64::PT_Flag::NX)
#define VMM_FLAGS_WRITE      ((size_t)1 << x86_64::PT_Flag::ReadWrite)
#define VMM_FLAGS_NOCACHE    ((size_t)1 << x86_64::PT_Flag::CacheDisabled)
#define VMM_FLAGS_USER       ((size_t)1 << x86_64::PT_Flag::UserSuper)
#define VMM_FLAGS_HUGE       ((size_t)1 << x86_64::PT_Flag::HugePages)

#define PTE_ADDR_MASK 0x000ffffffffff000
#define PTE_GET_ADDR(VALUE) ((VALUE) & PTE_ADDR_MASK)
#define PTE_GET_FLAGS(VALUE) ((VALUE) & ~PTE_ADDR_MASK)


