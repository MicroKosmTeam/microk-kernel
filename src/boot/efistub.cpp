#include <boot/boot.hpp>
#include <init/main.hpp>
#include <sys/printk.hpp>

extern "C" void EfiMain() {
	// TODO: Add EFI code

	EarlyKernelInit();

	PRINTK::PrintK("Hello from MicroK EFI Stub.\r\n");
}
