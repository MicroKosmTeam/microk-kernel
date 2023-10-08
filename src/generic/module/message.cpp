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

}
