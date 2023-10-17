#include <sys/io.hpp>

static inline int HandleSYSIORequest(IORequest *request);
static inline int HandleSYSGENERALRequest(IORequest *request);

static inline int HandleSYSIOByte(IORequest *request);
static inline int HandleSYSIOWord(IORequest *request);
static inline int HandleSYSIODWord(IORequest *request);
static inline int HandleSYSIOQWord(IORequest *request);

int ExecuteIORequest(IORequest requests[], usize requestCount) {
	for (usize currentRequest = 0; currentRequest < requestCount; ++currentRequest) {
		IORequest *request = &requests[currentRequest];

		u8 memoryType = request->MemoryAccessType & MEMORY_MASK_TYPE;

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
	u8 accessLength = request->MemoryAccessType & MEMORY_MASK_SIZE;

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
	u8 accessType = request->MemoryAccessType & MEMORY_MASK_ACCESS;

	switch(accessType) {
		case MEMORY_ACCESS_READ: {
			u8 inData = x86_64::InB(request->Address);
			CopyToUser(&inData, request->Data, sizeof(u8));
			}
			break;
		case MEMORY_ACCESS_WRITE:
			u8 outData;
			CopyFromUser(&outData, request->Data, sizeof(u8));
			x86_64::OutB(request->Address, outData);
			break;
	}

	return 0;
}

static inline int HandleSYSIOWord(IORequest *request) {
	u8 accessType = request->MemoryAccessType & MEMORY_MASK_ACCESS;

	switch(accessType) {
		case MEMORY_ACCESS_READ: {
			u16 inData = x86_64::InW(request->Address);
			CopyToUser(&inData, request->Data, sizeof(u16));
			}
			break;
		case MEMORY_ACCESS_WRITE:
			u16 outData;
			CopyFromUser(&outData, request->Data, sizeof(u16));
			x86_64::OutW(request->Address, outData);
			break;
	}

	return 0;
}

static inline int HandleSYSIODWord(IORequest *request) {
	u8 accessType = request->MemoryAccessType & MEMORY_MASK_ACCESS;

	switch(accessType) {
		case MEMORY_ACCESS_READ: {
			u32 inData = x86_64::InD(request->Address);
			CopyToUser(&inData, request->Data, sizeof(u32));
			}
			break;
		case MEMORY_ACCESS_WRITE:
			u32 outData;
			CopyFromUser(&outData, request->Data, sizeof(u32));
			x86_64::OutD(request->Address, outData);
			break;
	}

	return 0;
}

static inline int HandleSYSIOQWord(IORequest *request) {
	u8 accessType = request->MemoryAccessType & MEMORY_MASK_ACCESS;

	switch(accessType) {
		case MEMORY_ACCESS_READ: {
			u64 inData = x86_64::InB(request->Address);
			CopyToUser(&inData, request->Data, sizeof(u64));
			}
			break;
		case MEMORY_ACCESS_WRITE:
			u64 outData;
			CopyFromUser(&outData, request->Data, sizeof(u64));
			x86_64::OutQ(request->Address, outData);
			break;
	}

	return 0;
}
