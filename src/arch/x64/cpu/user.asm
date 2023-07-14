[bits 64]

extern SetAPICTimer

global EnterUserspace
EnterUserspace:
	cli
	mov rcx, rdi        ; first argument, new instruction pointer
	mov rsp, rsi        ; second argument, new stack pointer

	mov r11, 0x0202     ; eflags
	
	o64 sysret


