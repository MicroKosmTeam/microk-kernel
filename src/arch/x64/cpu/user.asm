[bits 64]

extern SetAPICTimer

global EnterUserspace
EnterUserspace:
	cli
;	lea rcx, UserStub
	mov rcx, rdi        ; first argument, new instruction pointer
	mov rsp, rsi        ; second argument, new stack pointer

	mov r11, 0x0202     ; eflags
	
	o64 sysret

	int 6

global UserStub
UserStub:
	pop rbp
	
	pop rax
	pop rbx
	pop rcx
	pop rdx

	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15

	pop rdi
	pop rsi

	o64 leave
	ret



