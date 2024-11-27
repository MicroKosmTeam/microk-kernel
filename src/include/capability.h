#pragma once
#include <stdint.h>
#include <stddef.h>

typedef enum : uint8_t {
	UntypedMemory,
	FrameMemory,
	CapabilitySpace,
	CapabilityNode
} object_kind;

typedef struct {
	object_kind kind;
	uintptr_t object;
} __attribute__((packed)) capability_t;

typedef struct cte_t {
	capability_t capability;
	cte_t *left, *right;
} cte_t;
