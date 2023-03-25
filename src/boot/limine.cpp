#include <limine.h>
#include <sys/panic.hpp>
#include <init/main.hpp>
#include <boot/boot.hpp>
#include <sys/printk.hpp>

extern "C" void LimineEntry(void);

static volatile limine_entry_point_request entryPointRequest {
	.id = LIMINE_ENTRY_POINT_REQUEST,
	.revision = 0,
	.entry = &LimineEntry
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
	EarlyKernelInit();

	PRINTK::PrintK("Welcome from MicroK Limine.\r\n");

	/* Checking if the stack request has been answered by Limine */
	/* If not, give up and shut down */
	if (stackRequest.response == NULL)
		PANIC("Stack size request not answered by Limine");

	KernelStart();
}
