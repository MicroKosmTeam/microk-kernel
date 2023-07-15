/*
   File: arch/x64/main.cpp
*/

#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <arch/aarch64/main.hpp>

namespace AArch64 {
void Init() {
	KInfo *info = GetInfo();

	while(true);
}
}

