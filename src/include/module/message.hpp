#pragma once
#include <stdint.h>
#include <stddef.h>

namespace MODULE {
	/* The header will always be 128 bytes */
	struct Message {
		uint32_t SenderVendorID : 32;
		uint32_t SenderProductID : 32;

		size_t MessageSize : 64;
	}__attribute__((packed));

	void ComposeMessage(Message *msg, uint32_t senderVendorID, uint32_t senderProductID, size_t dataSize);
	int SendMailboxMessage(uint32_t vendorID, uint32_t productID, uint32_t bufferID, Message *message);
};
