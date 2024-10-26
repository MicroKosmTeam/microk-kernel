[bits 64]

global GetRSP
GetRSP:
	mov rax, rsp
	ret

global GetRIP
GetRIP:
	mov rax, [rsp]
	ret
