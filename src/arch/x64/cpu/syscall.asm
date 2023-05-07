[bits 64]

global SyscallEntry
SyscallEntry:
	cli

.stop:
	hlt
	jmp .stop
