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
mov rax, [rsp - 8]
cmp rax, 0x28 
pop rax
je .done_swapgs_begin_%+%1
swapgs
.done_swapgs_begin_%+%1:
%endmacro

%macro swapgs_if_necessary_end 1
push rax
mov rax, [rsp - 8]
cmp rax, 0x28 
pop rax
je .done_swapgs_end_%+%1
swapgs
.done_swapgs_end_%+%1:
%endmacro

%macro exitisr 0
o64 iret
%endmacro
