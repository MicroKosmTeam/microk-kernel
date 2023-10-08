#pragma once
#include <stdint.h>
#include <stddef.h>
#include <cdefs.h>
#include <sys/user.hpp>
#include <sys/printk.hpp>

struct IORequest {
	uint8_t MemoryAccessType;
	uintptr_t Address;
	userptr_t Data;
}__attribute__((packed));

int ExecuteIORequest(IORequest requests[], size_t requestCount);
