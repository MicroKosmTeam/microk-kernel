#include <mm/memory.hpp>

extern "C" int memcmp(const void* buf1, const void* buf2, usize count) {
	return Memcmp(buf1, buf2, count);
}

int Memcmp(const void* buf1, const void* buf2, usize count) {
	if(!count)
		return(0);

	while(--count && *(char*)buf1 == *(char*)buf2 ) {
		buf1 = (char*)buf1 + 1;
		buf2 = (char*)buf2 + 1;
	}

	return(*((unsigned char*)buf1) - *((unsigned char*)buf2));
}