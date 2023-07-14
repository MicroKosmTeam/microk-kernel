#pragma once

int EarlyKernelInit();
void PrintBanner();
__attribute__((noreturn)) void KernelStart();
__attribute__((noreturn)) void RestInit();

