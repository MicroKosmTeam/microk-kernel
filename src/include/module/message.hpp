#pragma once
#include <stdint.h>
#include <stddef.h>

namespace MODULE {
	struct Message {
		uint32_t SenderVendorID;
		uint32_t SenderProductID;

		size_t MessageSize;
		uint8_t Data[];
	}__attribute__((packed));

	void Compose(Message *msg, uint32_t senderVendorID, uint32_t senderProductID);
	int SendDirect(uint32_t vendorID, uint32_t productID, Message *message , uint8_t *data, size_t size);
	int SendMailbox(uint32_t bufferID, Message *message, uint8_t *data, size_t size);
};
