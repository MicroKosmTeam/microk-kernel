#include <sys/io.hpp>

static inline int HandleSYSIORequest(IORequest *request);
static inline int HandleSYSGENERALRequest(IORequest *request);

static inline int HandleSYSIOByte(IORequest *request);
static inline int HandleSYSIOWord(IORequest *request);
static inline int HandleSYSIODWord(IORequest *request);
static inline int HandleSYSIOQWord(IORequest *request);

int ExecuteIORequest(IORequest requests[], size_t requestCount) {
	for (size_t currentRequest = 0; currentRequest < requestCount; ++currentRequest) {
		IORequest *request = &requests[currentRequest];

		uint8_t memoryType = request->MemoryAccessType & MEMORY_MASK_TYPE;

		switch (memoryType) {
			case MEMORY_SYSGENERAL:
				HandleSYSGENERALRequest(request);
				break;
			case MEMORY_SYSIO:
				HandleSYSIORequest(request);
				break;
			default:
				break;
		}
	}

	return 0;
}

static inline int HandleSYSIORequest(IORequest *request) {
	uint8_t accessLength = request->MemoryAccessType & MEMORY_MASK_SIZE;

	switch(accessLength) {
		case MEMORY_ACCESS_BYTE:
			HandleSYSIOByte(request);
			break;
		case MEMORY_ACCESS_WORD:
			HandleSYSIOWord(request);
			break;
		case MEMORY_ACCESS_DWORD:
			HandleSYSIODWord(request);
			break;
		case MEMORY_ACCESS_QWORD:
			HandleSYSIOQWord(request);
			break;
		default:
			break;
	}

	return 0;
}

static inline int HandleSYSGENERALRequest(IORequest *request) {
	(void)request;
	return -1;
}


static inline int HandleSYSIOByte(IORequest *request) {
	uint8_t accessType = request->MemoryAccessType & MEMORY_MASK_ACCESS;

	switch(accessType) {
		case MEMORY_ACCESS_READ: {
			uint8_t inData = x86_64::InB(request->Address);
			CopyToUser(&inData, request->Data, sizeof(uint8_t));
			}
			break;
		case MEMORY_ACCESS_WRITE:
			uint8_t outData;
			CopyFromUser(&outData, request->Data, sizeof(uint8_t));
			x86_64::OutB(request->Address, outData);
			break;
	}

	return 0;
}

static inline int HandleSYSIOWord(IORequest *request) {
	uint8_t accessType = request->MemoryAccessType & MEMORY_MASK_ACCESS;

	switch(accessType) {
		case MEMORY_ACCESS_READ: {
			uint16_t inData = x86_64::InW(request->Address);
			CopyToUser(&inData, request->Data, sizeof(uint16_t));
			}
			break;
		case MEMORY_ACCESS_WRITE:
			uint16_t outData;
			CopyFromUser(&outData, request->Data, sizeof(uint16_t));
			x86_64::OutW(request->Address, outData);
			break;
	}

	return 0;
}

static inline int HandleSYSIODWord(IORequest *request) {
	uint8_t accessType = request->MemoryAccessType & MEMORY_MASK_ACCESS;

	switch(accessType) {
		case MEMORY_ACCESS_READ: {
			uint32_t inData = x86_64::InD(request->Address);
			CopyToUser(&inData, request->Data, sizeof(uint32_t));
			}
			break;
		case MEMORY_ACCESS_WRITE:
			uint32_t outData;
			CopyFromUser(&outData, request->Data, sizeof(uint32_t));
			x86_64::OutD(request->Address, outData);
			break;
	}

	return 0;
}

static inline int HandleSYSIOQWord(IORequest *request) {
	uint8_t accessType = request->MemoryAccessType & MEMORY_MASK_ACCESS;

	switch(accessType) {
		case MEMORY_ACCESS_READ: {
			uint64_t inData = x86_64::InB(request->Address);
			CopyToUser(&inData, request->Data, sizeof(uint64_t));
			}
			break;
		case MEMORY_ACCESS_WRITE:
			uint64_t outData;
			CopyFromUser(&outData, request->Data, sizeof(uint64_t));
			x86_64::OutQ(request->Address, outData);
			break;
	}

	return 0;
}
