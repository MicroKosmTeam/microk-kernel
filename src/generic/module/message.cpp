#include <init/kinfo.hpp>
#include <proc/scheduler.hpp>
#include <module/message.hpp>
#include <module/buffer.hpp>
#include <module/modulemanager.hpp>

#include <sys/printk.hpp>
namespace MODULE {
void ComposeMessage(Message *msg, uint32_t senderVendorID, uint32_t senderProductID, size_t dataSize) {
	msg->SenderVendorID = senderVendorID;
	msg->SenderProductID = senderProductID;
	msg->MessageSize = dataSize;
}

int SendMailboxMessage(uint32_t vendorID, uint32_t productID, uint32_t bufferID, Message *message) {
	KInfo *info = GetInfo();

	size_t sizeofMessage = sizeof(Message) + message->MessageSize;

	Module *mod = info->KernelModuleManager->GetModule(vendorID, productID);
	if (mod == NULL) return -1;

	Buffer *buf = mod->GetBuffer(bufferID);
	if (buf == NULL) return -1;

	if(buf->Type != BT_MAILBOX) return -1;
	if(buf->Size < sizeofMessage) return -1;

	if(LockBuffer(buf) != 0) return -1;

	memcpy(buf->Address, message, sizeofMessage);

	if(UnlockBuffer(buf) != 0) return -1;

	size_t pid = mod->GetProcess()->GetPID();
	info->kernelScheduler->SetProcessState(pid, PROC::P_MESSAGE);

	return 0;
}
};
