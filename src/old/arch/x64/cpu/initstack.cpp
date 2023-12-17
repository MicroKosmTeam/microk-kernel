#include <arch/x64/cpu/stack.hpp>

#include <sys/panic.hpp>
void InitializeStack(void *stack, uptr function) {
	SaveContext *newStack = (SaveContext*)stack;

	newStack->RBP = (u64)&newStack->RBP2;
	newStack->ret = (u64)function;
}
