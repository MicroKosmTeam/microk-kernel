[bits 64]

%macro pushall 0
push rdi
push rsi

push rax
push rbx
push rcx
push rdx

push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15

%endmacro

%macro popall 0

pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8

pop rdx
pop rcx
pop rbx
pop rax

pop rsi
pop rdi

add rsp, 16

%endmacro

%macro swapgs_if_necessary_begin 1
push rax
mov rax, [rsp + 8]
cmp rax, 0x28 
je .done_swapgs_begin_%+%1
swapgs
.done_swapgs_begin_%+%1:
pop rax
%endmacro

%macro swapgs_if_necessary_end 1
push rax
mov rax, [rsp + 8]
cmp rax, 0x28 
je .done_swapgs_end_%+%1
swapgs
.done_swapgs_end_%+%1:
pop rax
%endmacro

%macro exitisr 0
o64 iret
%endmacro

%macro switch_to_kernel_cr3 1
	mov gs:40, rax
	mov rax, gs:32
	
	cmp rax, rax
	jz .completed_switch_kernel_%+%1
	
	mov cr3, rax

.completed_switch_kernel_%+%1:
	mov rax, gs:40
%endmacro

%macro switch_to_user_cr3 1
	mov gs:40, rax
	mov rax, gs:24

	cmp rax, rax
	jz .completed_switch_user_%+%1

	mov cr3, rax

.completed_switch_user_%+%1:
	mov rax, gs:40
%endmacro

%macro switch_to_kernel_stack 0
mov gs:8, rsp   ; Save the user stack pointer
mov gs:16, rbp  ; Save the user stack base pointer
mov rsp, gs:0	; Load the kernel stack pointer
%endmacro

%macro switch_to_user_stack 0
mov rsp, gs:8   ; Load the user stack pointer
mov rbp, gs:16  ; Load the user stack base pointer
%endmacro

