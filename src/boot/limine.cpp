#include <limine.h>
#include <sys/panic.hpp>
#include <init/main.hpp>
#include <boot/boot.hpp>
#include <sys/printk.hpp>

/* Function called by Limine */
extern "C" void LimineEntry(void);

/*
   Setting the correct entry point for Limine
*/
static volatile limine_entry_point_request entryPointRequest {
	.id = LIMINE_ENTRY_POINT_REQUEST,
	.revision = 0,
	.entry = &LimineEntry
};

/*
   System time request for Limine
*/
static volatile limine_boot_time_request timeRequest {
	.id = LIMINE_BOOT_TIME_REQUEST,
	.revision = 0
};

/*
   Bootloader info request for Limine
*/
static volatile limine_bootloader_info_request bootloaderRequest {
	.id = LIMINE_BOOTLOADER_INFO_REQUEST,
	.revision = 0
};

/*
   Stack request for Limine
   Stack size configured in autoconf.h
*/
static volatile limine_stack_size_request stackRequest {
	.id = LIMINE_STACK_SIZE_REQUEST,
	.revision = 0,
	.stack_size = CONFIG_STACK_SIZE
};

extern "C" void LimineEntry() {
	/* Startup basic kernel runtime services */
	EarlyKernelInit();

	/* Checking requests have been answered by Limine */
	/* If not, give up and shut down */
	if(timeRequest.response == NULL ||
	   stackRequest.response == NULL ||
	   bootloaderRequest.response == NULL)
		PANIC("Requests not answered by Limine");


	PRINTK::PrintK("Welcome from MicroK.\r\n"
		       "The kernel is booted by %s %s at %d\r\n",
		       bootloaderRequest.response->name,
		       bootloaderRequest.response->version,
		       timeRequest.response->boot_time);

	/* Launch the kernel proper */
	KernelStart();
}
