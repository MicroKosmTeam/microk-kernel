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
	(void)oldStack;
	(void)newStack;
	while(true);
}

void InitializeStack(void *stack, uptr function) {
	(void)stack;
	(void)function;
	while(true);
}

namespace AArch64 {
void EarlyInit() {
}

void Init() {

	while(true);
}
}

