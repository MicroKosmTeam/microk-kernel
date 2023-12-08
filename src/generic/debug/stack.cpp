#include <cstdint.hpp>

#include <debug/stack.hpp>
#include <debug/symbol.hpp>
#include <sys/panic.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>

uptr __stack_chk_guard = 0;

extern "C" __attribute__((noreturn))
void __stack_chk_fail() {
	PANIC("Stack smashing detected in the kernel");
}

/* Assume, as is often the case, that RBP is the first thing pushed. If not, we are in trouble. */
struct StackFrame {
	StackFrame *RBP;
	u64 RIP;
}__attribute__((packed));

void UnwindStack(int MaxFrames) {
	KInfo *info = GetInfo();
	(void)info;

	StackFrame *stk = (StackFrame*)__builtin_frame_address(0);
	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Stack trace:\r\n");

	for(int frame = 0; stk && frame < MaxFrames; ++frame) {
		if((uptr)stk->RBP < info->HigherHalfMapping || stk->RIP < info->KernelVirtualBase) {

			break;
		}

		/* Unwind to previous stack frame */
		const char *name = SYMBOL::AddressToSymbolName(stk->RIP);

		PRINTK::PrintK(PRINTK_DEBUG "  0x%x   %s\r\n", stk->RIP, name);

		stk = stk->RBP;
	}
}
