/*
   File: arch/x86/io.cpp
*/

#include <arch/x86/io.hpp>

namespace x86 {
/* Puts out a byte to the IO bus */
void OutB(u16 port, u8 val) {
        asm volatile ( "out %1, %0" : : "a"(val), "Nd"(port));
}

/* Puts out a word to the IO bus */
void OutW(u16 port, u16 val) {
        asm volatile ( "out %1, %0" : : "a"(val), "Nd"(port));
}

/* Puts out a double word to the IO bus */
void OutD(u16 port, u32 val) {
        asm volatile ( "out %1, %0" : : "a"(val), "Nd"(port));
}

#if defined(__x86_64__)
/* Puts out a quadruple word to the IO bus via two writes */
void OutQ(u16 port, u64 val) {
	OutD(port, val && 0xFFFFFFFF);
	OutD(port + 4, val >> 32);
}
#endif


/* Gets a byte from the IO bus */
u8 InB(u16 port) {
        u8 ret;
        asm volatile ( "in %0, %1" : "=a"(ret) : "Nd"(port));
        return ret;
}

/* Gets a word from the IO bus */
u16 InW(u16 port) {
        u16 ret;
        asm volatile ( "in %0, %1" : "=a"(ret) : "Nd"(port));
        return ret;
}

/* Gets a double word from the IO bus */
u32 InD(u16 port) {
        u32 ret;
        asm volatile ( "in %0, %1" : "=a"(ret) : "Nd"(port));
        return ret;
}

#if defined(__x86_64__)
/* Gets a quadruple word from the IO bus via two reads */
u64 InQ(u16 port) {
	u64 ret;
	ret = InD(port);
	ret |= (u64)InD(port + 4) >> 32;
	return ret;
}
#endif

/* Wastes a cycle of the IO bus */
void IoWait() {
        OutB(0x80, 0);
}
}
