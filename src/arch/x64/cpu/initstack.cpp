#include <arch/x64/cpu/stack.hpp>


void InitializeStack(void *stack, uintptr_t function) {
	SaveContext *newStack = stack;
	newStack->RBP = (uint64_t)&newStack->RBP2;
	newStack->ret = (uint64_t)function;

}
