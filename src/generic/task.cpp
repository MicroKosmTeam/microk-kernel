#include <task.hpp>
#include <memory.hpp>

static u64 LastID = 0;
static u64 RequestID() {
	return ++LastID;
}

namespace TASK {
ThreadControlBlock *InitializeTCB(uptr frame) {
	ThreadControlBlock *tcb = (ThreadControlBlock*)frame;
	Memclr(tcb, PAGE_SIZE);

	tcb->Status = ThreadStatus::RUNNING;
	tcb->TaskID = RequestID();
	tcb->Priority = SCHEDULER_MIN_PRIORITY;

	return tcb;
}
}


