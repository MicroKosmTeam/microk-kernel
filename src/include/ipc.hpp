#pragma once
#include <object.hpp>

namespace IPC {
	Endpoint *InitializeEndpoint(uptr frame);
	int SendMessage(ThreadControlBlock *sendTCB, Capability *epCap);
}
