/*
   File: arch/x64/main.cpp
*/

#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <arch/aarch64/main.hpp>

extern "C" void __aarch64_cas1_sync() {
	while(true);
}

extern "C" void CPUPause() {
	while(true);
}

extern "C" void EnterUserspace() {
	while(true);
}

extern "C" void SwitchStack(void *oldStack, void *newStack) {
	while(true);
}

void InitializeStack(void *stack, uintptr_t function) {
	while(true);
}

namespace AArch64 {
void EarlyInit() {
	KInfo *info = GetInfo();
}

void Init() {
	KInfo *info = GetInfo();

	while(true);
}
}

