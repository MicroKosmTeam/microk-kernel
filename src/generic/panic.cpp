#include <printk.hpp>

__attribute__((noreturn))
void Panic(const char *message, const char *file, const char *function, unsigned int line) {
        // Printing the panic message
	PRINTK::PrintK(PRINTK_DEBUG "\r\n\r\n"
		       " [PANIC] KERNEL PANIC\r\n"
		       " [PANIC] Irrecoverable error in the kernel.\r\n"
		       " [PANIC] %s in function %s at line %d\r\n"
		       " [PANIC] Cause: %s\r\n\r\n",
		       file, function, line, message);

	PRINTK::PrintK(PRINTK_DEBUG "\r\n [Hanging now...]\r\n\r\n");
	
        while (true);

	__builtin_unreachable();
}

void Oops(const char *message, const char *file, const char *function, unsigned int line) {
        // Printing the panic message
	PRINTK::PrintK(PRINTK_DEBUG "\r\n\r\n"
		       " [OOPS] KERNEL OOPS\r\n"
		       " [OOPS] Error in the kernel.\r\n"
		       " [OOPS] %s in function %s at line %d\r\n"
		       " [OOPS] Cause: %s\r\n\r\n",
		       file, function, line, message);

	PRINTK::PrintK(PRINTK_DEBUG "\r\n [Continuing execution...]\r\n\r\n");
}
