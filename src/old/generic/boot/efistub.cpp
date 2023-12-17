#include <boot/boot.hpp>
#include <init/main.hpp>
#include <sys/printk.hpp>

extern "C" void EfiMain() {
	PRINTK::PrintK(PRINTK_DEBUG "Hello from MicroK EFI Stub.\r\n");

	return;
}
