#include <ipc.hpp>
#include <memory.hpp>
#include <sched.hpp>

namespace IPC {
Endpoint *InitializeEndpoint(uptr frame, ThreadControlBlock *thread) {
	Endpoint *ep = (Endpoint*)frame;

	ep->Thread = thread;
	ep->Status = EndpointStatus::EP_IDLE;
	ep->ThreadQueue.Head = ep->ThreadQueue.Tail = NULL;

	return ep;
}

int SendMessage(ThreadControlBlock *sendTCB, Capability *epCap) {
	Endpoint *ep = (Endpoint*)epCap->Object;

	if (ep->Status != EndpointStatus::EP_RECV) {
		return -EBUSY;
	}

	ThreadControlBlock *recvTCB = ep->Thread;

	switch (recvTCB->Status) {
		case ThreadStatus::RUNNING:
			/* TODO */
			return -EBUSY;
		case ThreadStatus::WAITING: {
			ep->Status = EndpointStatus::EP_IDLE;

			Memcpy(recvTCB->VirtualRegisters, sendTCB->VirtualRegisters, VIRTUAL_REGISTERS_SIZE);

			SCHED::RemoveThread(sendTCB->Parent, sendTCB);
			sendTCB->Status = ThreadStatus::BLOCKED;
			SCHED::AddThread(sendTCB->Parent, sendTCB);
			}
			break;
		case ThreadStatus::BLOCKED:
			return -EBUSY;
		default:
			return -EINVALID;
	}

	return 0;
}

}
