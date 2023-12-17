#pragma once
#include <cstdint.hpp>

#include <cdefs.h>
#include <sys/user.hpp>
#include <sys/printk.hpp>

#if defined(__x86_64__)
#include <arch/x64/io/io.hpp>
#endif

struct IORequest {
	u8 MemoryAccessType;
	uptr Address;
	userptr_t Data;
}__attribute__((packed));

int ExecuteIORequest(IORequest requests[], usize requestCount);

#if !defined(__aarch64__)
inline void OutB(uptr port, u8 byte) {
#if defined(__x86_64__)
	x86_64::OutB(port, byte);
#endif
}

inline u8 InB(uptr port) {
#if defined(__x86_64__)
	return x86_64::InB(port);
#endif
}

#endif

