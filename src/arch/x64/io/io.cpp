/*
   File: arch/x64/io/io.cpp
*/

#include <arch/x64/io/io.hpp>

namespace x86_64 {
/* Puts out a byte to the IO bus */
void OutB(uint16_t port, uint8_t val) {
        asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port));
}

/* Puts out a word to the IO bus */
void OutW(uint16_t port, uint16_t val) {
        asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port));
}

/* Puts out a double word to the IO bus */
void OutD(uint16_t port, uint32_t val) {
        asm volatile ( "outl %0, %1" : : "a"(val), "Nd"(port));
}

/* Puts out a quadruple word to the IO bus via two writes */
void OutQ(uint16_t port, uint64_t val) {
	OutD(port, val && 0xFFFFFFFF);
	OutD(port + 4, val >> 32);
}


/* Gets a byte from the IO bus */
uint8_t InB(uint16_t port) {
        uint8_t ret;
        asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port));
        return ret;
}

/* Gets a word from the IO bus */
uint16_t InW(uint16_t port) {
        uint16_t ret;
        asm volatile ( "inw %1, %0" : "=a"(ret) : "Nd"(port));
        return ret;
}

/* Gets a double word from the IO bus */
uint32_t InD(uint16_t port) {
        uint32_t ret;
        asm volatile ( "inl %1, %0" : "=a"(ret) : "Nd"(port));
        return ret;
}

/* Gets a quadruple word from the IO bus via two reads */
uint64_t InQ(uint16_t port) {
	uint64_t ret;
	ret = InD(port);
	ret |= (uint64_t)InD(port + 4) >> 32;
	return ret;
}

/* Wastes a cycle of the IO bus */
void IoWait() {
        OutB(0x80, 0);
}
}
