
GLOBAL _int80h
section .text

_hlt:
	hlt
	ret

%macro pushrg 0
	push rbp
	mov rbp,rsp

	push rbx
	push rbp
	push r12
	push r13
	push r14
	push r15
%endMacro

%macro poprg 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbp
	pop rbx

	leave
%endMacro

;RDI, RSI, RDX, RCX, R8, R9
_int80h:
	pushrg

	mov rax,rdi
	mov rdi,rsi
	mov rsi,rdx
	mov rdx,rcx
	int 80h

	poprg
	ret
