#include <arch/x64/cpu/stack.hpp>

#include <sys/printk.hpp>
void InitializeStack(void *stack) {
	SaveContext *newStack = stack;

	newStack->RBP = (uint64_t)&newStack->RBP;
}
