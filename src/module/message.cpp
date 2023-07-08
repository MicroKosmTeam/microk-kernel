#include <init/kinfo.hpp>
#include <module/message.hpp>
#include <module/buffer.hpp>
#include <module/modulemanager.hpp>

namespace MODULE {
void Compose(Message *msg, uint32_t senderVendorID, uint32_t senderProductID) {
	msg->SenderVendorID = senderVendorID;
	msg->SenderProductID = senderProductID;
}

int SendDirect(uint32_t vendorID, uint32_t productID, Message *message , uint8_t *data, size_t size) {
	return 0;
}

int SendMailbox(uint32_t bufferID, Message *message, uint8_t *data, size_t size) {
	KInfo *info = GetInfo();

	size_t sizeofMessage = sizeof(Message);

	Module *mod = info->KernelModuleManager->GetModule(message->SenderVendorID, message->SenderProductID);
	if (mod == NULL) return -1;

	Buffer *buf = mod->GetBuffer(bufferID);
	if (buf == NULL) return -1;

	if(buf->Type != BT_MAILBOX) return -1;
	if(buf->Size < sizeofMessage + size) return -1;

	message->MessageSize = size;

	if(LockBuffer(buf) != 0) return -1;

	memcpy(buf->Address, message, sizeofMessage);
	memcpy(buf->Address + sizeofMessage, data, size);

	if(UnlockBuffer(buf) != 0) return -1;

	return 0;
}
};
