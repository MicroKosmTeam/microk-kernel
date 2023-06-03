#include <arch/x64/cpu/stack.hpp>

#include <sys/panic.hpp>
void InitializeStack(PROC::Thread *thread, uintptr_t function) {
	OOPS("Stack initializing not implemented");
	SaveContext *newStack/* = thread->Stack*/;

	newStack->RBP = (uint64_t)&newStack->RBP2;
	newStack->RDI = (uint64_t)thread;
	newStack->ret = (uint64_t)function;
}
