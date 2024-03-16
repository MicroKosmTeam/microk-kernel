#include <task.hpp>
#include <memory.hpp>

static u64 LastID = 0;
static u64 RequestID() {
	return ++LastID;
}

namespace TASK {
ThreadControlBlock *InitializeTCB(uptr frame) {
	ThreadControlBlock *tcb = (ThreadControlBlock*)frame;
	Memclr(tcb, sizeof(ThreadControlBlock));

	tcb->Status = ThreadStatus::RUNNING;
	tcb->TaskID = RequestID();
	tcb->Priority = SCHEDULER_MIN_PRIORITY;

	return tcb;
}

SchedulerContext *InitializeContext(uptr frame, uptr ip, uptr sp, uptr argc, uptr argv) {
	SchedulerContext *context = (SchedulerContext*)frame;
	Memclr(context, sizeof(SchedulerContext));

	context->IP = ip;
	context->BP = context->SP = sp;

#if defined(__x86_64__)
	context->RFLAGS = 0x202;

	context->Registers.RDI = argc;
	context->Registers.RSI = argv;
#endif

	return context;
}
}


