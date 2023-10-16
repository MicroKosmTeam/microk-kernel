#pragma once
#include <stdint.h>
#include <stddef.h>
#include <cdefs.h>
#include <sys/user.hpp>
#include <sys/printk.hpp>

#if defined(ARCH_x64)
#include <arch/x64/io/io.hpp>
#endif

struct IORequest {
	uint8_t MemoryAccessType;
	uintptr_t Address;
	userptr_t Data;
}__attribute__((packed));

int ExecuteIORequest(IORequest requests[], size_t requestCount);

#if !defined(ARCH_AArch64)
inline void OutB(uintptr_t port, uint8_t byte) {
#if defined(ARCH_x64)
	x86_64::OutB(port, byte);
#endif
}

inline uint8_t InB(uintptr_t port) {
#if defined(ARCH_x64)
	return x86_64::InB(port);
#endif
}

#endif

