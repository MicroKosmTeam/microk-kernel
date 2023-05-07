#include <sys/printk.hpp>
#include <debug/stack.hpp>
#include <stdint.h>

__attribute__((noreturn))
void Panic(const char *message, const char *file, const char *function, unsigned int line) {
        asm volatile ("cli"); // We don't want interrupts while we are panicking

	uint64_t cr2;
	asm volatile ("mov %0, %%cr2" : "=r"(cr2));
	PRINTK::PrintK("CR2: 0x%x\r\n", cr2);

        // Printing the panic message
	PRINTK::PrintK("\r\n\r\n KERNEL PANIC!! \r\n");
	PRINTK::PrintK("Irrecoverable error in the kernel.\r\n\r\n");
	PRINTK::PrintK("%s in function %s at line %d\r\n", file, function, line);
	PRINTK::PrintK("Cause: %s\r\n", message);
	UnwindStack(64);
	PRINTK::PrintK("[Hanging now...]\r\n");

        while (true) {
                // Halting forever
                asm volatile ("hlt");
        }

}
