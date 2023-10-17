#pragma once
#include <cstdint.hpp>

#include <cdefs.h>
#include <sys/user.hpp>
#include <sys/printk.hpp>

#if defined(ARCH_x64)
#include <arch/x64/io/io.hpp>
#endif

struct IORequest {
	u8 MemoryAccessType;
	uptr Address;
	userptr_t Data;
}__attribute__((packed));

int ExecuteIORequest(IORequest requests[], usize requestCount);

#if !defined(ARCH_AArch64)
inline void OutB(uptr port, u8 byte) {
#if defined(ARCH_x64)
	x86_64::OutB(port, byte);
#endif
}

inline u8 InB(uptr port) {
#if defined(ARCH_x64)
	return x86_64::InB(port);
#endif
}

#endif

