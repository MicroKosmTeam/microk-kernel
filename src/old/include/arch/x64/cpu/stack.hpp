#pragma once
#include <cstdint.hpp>


struct SaveContext {
	u64 RBP;

	u64 R15;
	u64 R14;
	u64 R13;
	u64 R12;
	u64 R11;
	u64 R10;
	u64 R9;
	u64 R8;

	u64 RDX;
	u64 RCX;
	u64 RBX;
	u64 RAX;

	u64 RSI;
	u64 RDI;
	
	u64 RBP2;
	u64 ret;
}__attribute__((packed));

extern "C" void SwitchStack(void *oldStack, void *newStack);
void InitializeStack(void *stack, uptr function);
