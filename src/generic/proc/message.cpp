#include <proc/message.hpp>

namespace PROC {
isize IPCMessageSend(usize queue, u8 *messagePointer, usize messageLength, usize messageFlags) {
	(void)queue;
	(void)messagePointer;
	(void)messageLength;
	(void)messageFlags;
	return 0;
}

isize IPCMessageReceive(usize queue, u8 *messageBufferPointer, usize maxMessageLength, usize messageType, usize messageFlags) {
	(void)queue;
	(void)messageBufferPointer;
	(void)maxMessageLength;
	(void)messageType;
	(void)messageFlags;
	return 0;
}

}
