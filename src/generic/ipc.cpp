#include <ipc.hpp>

namespace IPC {
Endpoint *InitializeEndpoint(uptr frame) {
	Endpoint *ep = (Endpoint*)frame;

	ep->Status = EndpointStatus::EP_IDLE;
	ep->ThreadQueue.Head = ep->ThreadQueue.Tail = NULL;

	return ep;
}

}
