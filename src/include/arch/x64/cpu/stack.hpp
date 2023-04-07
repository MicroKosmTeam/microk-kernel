#pragma once
#include <stdint.h>

struct ThreadSaveStack {
	uint64_t RBP;
	uint64_t RBX;
	uint64_t R12;
	uint64_t R13;
	uint64_t R14;
	uint64_t R15;
	uint64_t RBP2;
	uint64_t Ret;
};

extern "C" void SwitchStack(void *oldStack, void *newStack);
