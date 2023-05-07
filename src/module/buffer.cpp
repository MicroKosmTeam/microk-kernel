#include <module/buffer.hpp>
#include <mm/pmm.hpp>
#include <mm/memory.hpp>
#include <sys/printk.hpp>
#include <sys/mutex.hpp>

namespace MODULE {
namespace Buffer { 
MKMI_Buffer *Create(uint64_t code, MKMI_BufferType type, size_t size) {
	MKMI_Buffer *destBuffer;

	switch(type) {
		case COMMUNICATION_MODULE_KERNEL:
		case COMMUNICATION_INTERMODULE:
		case DATA_MODULE_GENERIC: {
			destBuffer = Malloc(size + sizeof(MKMI_Buffer) + 1);
			memset(destBuffer, 0, size);
			destBuffer->address = (uintptr_t)destBuffer;
			}
			break;
		default:
			return NULL;
	}

	destBuffer->type = type;
	destBuffer->size = size;

	UnlockMutex(&destBuffer->lock);

	return destBuffer;
}

uint64_t IO(MKMI_Buffer *buffer, MKMI_BufferOperation operation, ...) {
	if (buffer == NULL) return 0;

	LockMutex(&buffer->lock);

	uint64_t result;

	va_list ap;
	va_start(ap, operation);

	switch(operation) {
		case OPERATION_READDATA: {
			uint64_t *destBuffer = va_arg(ap, uint64_t*);
			size_t destBufferLength = va_arg(ap, size_t);

			uint64_t *startBuffer = (uint64_t*)buffer->address + sizeof(MKMI_Buffer);
			size_t readSize =  buffer->size > destBufferLength ? destBufferLength : buffer->size;

			memcpy(destBuffer, startBuffer,	readSize);
			result = 1;
			}
			break;
		case OPERATION_WRITEDATA: {
			uint64_t *startBuffer = va_arg(ap, uint64_t*);
			uint64_t *destBuffer = (uint64_t*)buffer->address + sizeof(MKMI_Buffer);
			size_t startBufferLength = va_arg(ap, size_t);
			size_t readSize =  buffer->size > startBufferLength ? startBufferLength : buffer->size;
			memcpy(destBuffer, startBuffer,	readSize);
			result = 2;
			}
			break;
		default:
			result = 0;
			break;
	}

	UnlockMutex(&buffer->lock);

	va_end(ap);

	return result;
}

uint64_t Delete(MKMI_Buffer *buffer) {
	if (buffer == NULL) return 0;

	LockMutex(&buffer->lock);

	switch(buffer->type) {
		case COMMUNICATION_MODULE_KERNEL:
		case COMMUNICATION_INTERMODULE:
		case DATA_MODULE_GENERIC: {
			uintptr_t address = buffer->address;
			size_t size = buffer->size + sizeof(MKMI_Buffer);
			memset(address, 0, size);
			Free(address);
			}
			break;
	}

	return 0;
}

}
}
