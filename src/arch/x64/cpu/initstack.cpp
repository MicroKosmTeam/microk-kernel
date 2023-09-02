#include <arch/x64/cpu/stack.hpp>

#include <sys/panic.hpp>
void InitializeStack(void *stack, uintptr_t function) {
	SaveContext *newStack = (SaveContext*)stack;

	newStack->RBP = (uint64_t)&newStack->RBP2;
	newStack->ret = (uint64_t)function;
}
