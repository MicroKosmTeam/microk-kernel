#include <proc/message.hpp>
#include <sys/printk.hpp>

namespace PROC {
static MessageQueue *CreateMessageQueue(MessageManager *manager, ProcessBase *owner, usize preallocateSize) {
	MessageQueue *queue = (MessageQueue*)Malloc(sizeof(MessageQueue) + preallocateSize);

	queue->Owner = owner;
	queue->FreeSize = queue->AllocatedSize = preallocateSize;
	queue->WaitingMessages = 0;
	queue->LastReceive = queue->LastSend = -1;
	queue->FirstFreeByteOffset = (uptr)queue->MessageStart - (uptr)queue;

	manager->Queues[manager->TotalQueues] = queue;
	queue->ID = manager->TotalQueues++;
	
	PRINTK::PrintK(PRINTK_DEBUG "Queue %d created with preallocated size of %d bytes.\r\n", queue->ID, queue->AllocatedSize);

	return queue;
}

MessageManager *IPCMessageManagerInitialize() {
	MessageManager *manager = new MessageManager;

	manager->TotalQueues = 0;
	manager->Queues = (MessageQueue**)new uptr[32];

	return manager;
}

isize IPCMessageQueueCtl(MessageManager *manager, QueueOperations operation, ...) {
	va_list ap;
	va_start(ap, operation);

	MessageQueue *queue;
	isize returnVal;

	switch (operation) {
		case QueueOperations::CREATE: {
			ProcessBase *proc = (ProcessBase*)va_arg(ap, uptr);
			usize preallocateSize = va_arg(ap, usize);
			queue = CreateMessageQueue(manager, proc, preallocateSize);
			if (queue != NULL) {
				returnVal = queue->ID;
			} else {
				returnVal = -EFAULT;
			}
			}

			break;
		default:
			returnVal = -EINVALID;
			break;
	}

	va_end(ap);

	return returnVal;
}

isize IPCMessageSend(MessageManager *manager, usize queueID, ProcessBase *proc, const u8 *messagePointer, usize messageLength, usize messageType, usize messageFlags) {
	MessageQueue *queue = manager->Queues[queueID];
	Message *message = (Message*)((uptr)queue + queue->FirstFreeByteOffset);
	
	if (queue->FreeSize < messageLength + sizeof(Message)) {
		return -ENOMEMORY;
	}
	
	message->Magic = MESSAGE_HEADER_MAGIC;
	message->SendTime = 0;
	message->ReceiveTime = 0;
	message->Length = messageLength;

	if (proc != NULL) {
		message->ProcessID = proc->ID;
	} else {
		message->ProcessID = 0;
	}

	Memcpy(message->MessageStart, (void*)messagePointer, messageLength);

	++queue->WaitingMessages;
	usize moveLength = messageLength + sizeof(Message);
	queue->FirstFreeByteOffset += moveLength;
	queue->FreeSize -= moveLength;

	(void)messageType;
	(void)messageFlags;

	return messageLength;
}

isize IPCMessageReceive(MessageManager *manager, usize queueID, ProcessBase *proc, u8 *messageBufferPointer, usize maxMessageLength, usize messageType, usize messageFlags) {
	if (manager->TotalQueues < queueID) {
		return -EINVALID;
	}

	MessageQueue *queue = manager->Queues[queueID];
	if (queue->WaitingMessages == 0) {
		return 0;
	}

	Message *message = (Message*)queue->MessageStart;
	if (message->Magic != MESSAGE_HEADER_MAGIC) {
		return -EINVALID;
	}

	usize length = message->Length > maxMessageLength ? maxMessageLength : message->Length;
	Memcpy(messageBufferPointer, message->MessageStart, length);

	usize moveLength = message->Length + sizeof(Message);
	Memmove(queue->MessageStart, (const void*)((uptr)queue->MessageStart + moveLength), queue->AllocatedSize - moveLength);
	queue->FirstFreeByteOffset -= moveLength;
	queue->FreeSize += moveLength;
	--queue->WaitingMessages;

	(void)proc;
	(void)messageType;
	(void)messageFlags;

	return length;
}

}
