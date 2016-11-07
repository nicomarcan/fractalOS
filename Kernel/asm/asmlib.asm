GLOBAL cpuVendor,_rax,_rbx,_rcx,_rdx,_rbp,_rsi,_rdi,_rsp
GLOBAL _r8,_r9,_r10,_r11,_r12,_r13,_r14,_r15,
GLOBAL _cli,_sti,_eax,_ebx,_ecx,_edx
GLOBAL _lidt,picMasterMask,picSlaveMask,_irq00handler,_irq01handler
GLOBAL _int80handler,kb_read,rtc,_out,_in
GLOBAL _syscall_handler,_hlt,_lhlt,_readfl,_rip,_lrax,changeContextFromRsp,enter_region,leave_region,yield
EXTERN irqDispatcher, int_80, syscall_dispatcher, switchStackPointer
EXTERN decrementTicks

section .text


enter_region:
	xor rdx,rdx
	xor rax,rax
	xor rcx,rcx
	xor rbx,rbx
	inc rbx
	lock cmpxchg8b [rdi]
	jnz .yid
	ret
.yid:
	call yield
	jmp enter_region
	ret

leave_region:
	mov QWORD [rdi],0
	ret

_readfl:
	push rbp
	mov rbp, rsp
	mov rax,0
	pushf
	pop ax
	leave
	ret

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

_eax:
	ret

_ebx:
	mov rax,0
	mov eax,ebx
	ret

_ecx:
	mov rax,0
	mov eax,ecx
	ret

_edx:
	mov rax,0
	mov eax,edx
	ret

_rip:
	mov rax,$
	ret

_lrax:
	mov rax,rdi
	ret

_rax:
	ret

_rbx:
	mov rax,rbx
	ret

_rcx:
	mov rax,rcx
	ret

_rdx:
	mov rax,rdx
	ret

_rbp:
	mov rax,rbp
	ret

_rsi:
	mov rax,rsi
	ret

_rdi:
	mov rax,rdi
	ret

_rsp:
	mov rax,rsp
	ret

_r8:
	mov rax,r8
	ret

_r9:
	mov rax,r9
	ret

_r10:
	mov rax,r10
	ret

_r11:
	mov rax,r11
	ret

_r12:
	mov rax,r12
	ret

_r13:
	mov rax,r13
	ret

_r14:
	mov rax,r14
	ret

_r15:
	mov rax,r15
	ret


_lidt:
	cli
	lidt [rdi]
	sti
	ret

picMasterMask:
	mov al,[rdi]
	out 21h,al
	retn

picSlaveMask:
	mov al,[rdi]
	out 0A1h,al
	retn

%macro irqHandlerMaster 1
	cli
	push rbp
	mov rbp, rsp

	push rbx
	push rbp
	push r12
	push r13
	push r14
	push r15
	mov rdi, %1
	call irqDispatcher
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbp
	pop rbx

	;signal pic
	mov al, 20h
	out 20h, al

	mov rax, 0
	leave
	sti
	iretq
%endMacro

%macro	pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push fs
	push gs
%endmacro

%macro	popState 0
	pop gs
	pop fs
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

_irq00handler:
	pushState

	;push rsp
	;mov rdi, 0
	;call irqDispatcher
	;pop rsp

	call decrementTicks

	mov rdi,rsp

	call switchStackPointer

	mov rsp,rax

	mov al, 20h
	out 20h, al

	popState
	iretq

yield:

	;push iretq hook

	mov rax,rsp
	push QWORD 0
	push QWORD 0
	push rax
	pushfq
	push QWORD 0x008
	push .ret

	;uint64_t rip
	;uint64_t cs
	;uint64_t eflags
	;uint64_t rsp
	;uint64_t ss
	;uint64_t base

	pushState
	mov rdi,rsp
	call switchStackPointer
	mov rsp,rax
	popState
	iretq
.ret:
	ret

;see exit syscall
changeContextFromRsp:
	mov rsp,rax
	popState
	iretq


_irq01handler:
	irqHandlerMaster 1

_syscall_handler:
	push rbp
	mov rbp, rsp
	push rcx
	push rdi
	push rdx
	push rsi

	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, rax
	xor rax, rax

	call syscall_dispatcher

	pop rsi
	pop rdx
	pop rdi
	pop rcx

	mov rsp, rbp;
	pop rbp
	iretq


_cli:
	cli
	ret

_sti:
	sti
	ret

kb_read:
	in al,64h
	test al,1
	jz .none
	mov rax,0
	in al,60h
	jmp .ret
.none:
	mov rax,0
.ret:
	ret

_hlt:
	hlt
	ret

_lhlt:
	hlt
	jmp _lhlt
	ret

rtc:
	call bin_fmt
	mov rax,rdi
	out 70h,al
	in al,71h
	ret

bin_fmt:
	push rbp
	mov rbp,rsp
	mov al,0bh
	out 70h,al
	in al,71h
	push rax
	mov al,0bh
	out 70h,al
	pop rax
	or al,04h
	out 71h,al
	mov rsp,rbp
	pop rbp
	ret


_in:
	mov rax,0
	mov rdx,rdi
	in ax,dx
	ret

;rdi port
;rsi value
_out:
	mov rdx,rdi
	mov rax,rsi
	out dx,ax
	ret
