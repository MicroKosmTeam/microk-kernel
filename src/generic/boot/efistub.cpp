#include <boot/boot.hpp>
#include <init/main.hpp>
#include <sys/printk.hpp>

extern "C" void EfiMain() {
	EarlyKernelInit();

	PRINTK::PrintK(PRINTK::INFO, MODULE_NAME, "Hello from MicroK EFI Stub.\r\n");

	return;
}
