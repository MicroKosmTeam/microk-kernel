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
		Process *Owner;

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

	struct QueueOperationStruct {
		QueueOperations Operation;
		
		union {
			struct {
				usize PreallocateSize;
				usize NewID;
			} Create;
		};
	}__attribute__((packed));

	MessageManager *IPCMessageManagerInitialize();
	int IPCMessageQueueCtl(MessageManager *manager, Process *proc, QueueOperationStruct *ctlStruct);
	int IPCMessageSend(MessageManager *manager, usize queueID, Process *proc, const u8 *messagePointer, usize messageLength, usize messageType, usize messageFlags);
	int IPCMessageReceive(MessageManager *manager, usize queueID, Process *proc, u8 *messageBufferPointer, usize maxMessageLength, usize messageType, usize messageFlags);

}
