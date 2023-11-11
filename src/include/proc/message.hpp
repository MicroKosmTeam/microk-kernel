#pragma once
#include <proc/process.hpp>
#include <sys/timer.hpp>

namespace PROC {
	struct Message {
		usize ProcessID;
		stime SendTime;
		stime ReceiveTime;
	};

	struct MessageQueue {
		usize ID;
		ProcessBase *Owner;

		usize DataSize;
		usize WaitingMessages;

		stime LastSend;
		stime LastReceive;

		usize FirstFreeByteOffset;
		
		u8 MessageStart[1];
	};

	isize IPCMessageSend(usize queue, u8 *messagePointer, usize messageLength, usize messageFlags);
	isize IPCMessageReceive(usize queue, u8 *messageBufferPointer, usize maxMessageLength, usize messageType, usize messageFlags);
	isize IPCMessageQueue(usize queue, usize operation);

}
