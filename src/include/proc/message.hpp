#pragma once
#include <proc/process.hpp>
#include <sys/timer.hpp>

#define MESSAGE_HEADER_MAGIC 0xCA11CAFE

namespace PROC {
	struct Message {
		u32 Magic;
		usize ProcessID;

		stime SendTime;
		stime ReceiveTime;

		usize Length;

		u8 MessageStart[1];
	}__attribute__((packed));

	struct MessageQueue {
		usize ID;
		ProcessBase *Owner;

		usize AllocatedSize;
		usize FreeSize;
		usize WaitingMessages;

		utime LastSend;
		utime LastReceive;

		usize FirstFreeByteOffset;
		
		u8 MessageStart[1];
	}__attribute__((packed));

	struct MessageManager {
		usize TotalQueues;

		MessageQueue **Queues;
	};


	enum QueueOperations {
		CREATE = 1,
	};

	MessageManager *IPCMessageManagerInitialize();
	isize IPCMessageQueueCtl(MessageManager *manager, QueueOperations operation, ...);
	isize IPCMessageSend(MessageManager *manager, usize queueID, ProcessBase *proc, const u8 *messagePointer, usize messageLength, usize messageType, usize messageFlags);
	isize IPCMessageReceive(MessageManager *manager, usize queueID, ProcessBase *proc, u8 *messageBufferPointer, usize maxMessageLength, usize messageType, usize messageFlags);

}
