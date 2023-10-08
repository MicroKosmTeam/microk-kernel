#pragma once
#include <stdint.h>
#include <stddef.h>

struct SaveContext {
	uint64_t RBP;

	uint64_t R15;
	uint64_t R14;
	uint64_t R13;
	uint64_t R12;
	uint64_t R11;
	uint64_t R10;
	uint64_t R9;
	uint64_t R8;

	uint64_t RDX;
	uint64_t RCX;
	uint64_t RBX;
	uint64_t RAX;

	uint64_t RSI;
	uint64_t RDI;
	
	uint64_t RBP2;
	uint64_t ret;
}__attribute__((packed));

extern "C" void SwitchStack(void *oldStack, void *newStack);
void InitializeStack(void *stack, uintptr_t function);
