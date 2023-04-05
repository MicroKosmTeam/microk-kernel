[bits 64]

global EnterUserspace
EnterUserspace:
	mov rcx, rdi        ; first argument, new instruction pointer
	mov rsp, rsi        ; second argument, new stack pointer
	mov r11, 0x0202     ; eflags
	o64 sysret

global ExitUserspace 
ExitUserspace:
	; Not working yet
	ret
	mov r11, 0x202
	o64 syscall
