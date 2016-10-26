GLOBAL read, write, time, sleep, memory, clear, set_color, set_back_color, pixel

section .text

memory:
  push rbp
  mov rbp, rsp

  push rbx
  push rbp
  push r12
  push r13
  push r14
  push r15

  mov rax, 8
  int 80h

  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx

  leave
  ret


write:
  push rbp
  mov rbp, rsp

  push rbx
  push rbp
  push r12
  push r13
  push r14
  push r15

  mov rax, 1
  int 80h

  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx

  leave
  ret

read:
  push rbp
  mov rbp, rsp

  push rbx
  push rbp
  push r12
  push r13
  push r14
  push r15

  mov rax, 0
  int 80h

  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx

  leave
  ret

time:
  push rbp
  mov rbp, rsp

  push rbx
  push rbp
  push r12
  push r13
  push r14
  push r15

  mov rax, 12
  int 80h

  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx

  leave
  ret

sleep:
	push rbp
	mov rbp,rsp

  push rbx
  push rbp
  push r12
  push r13
  push r14
  push r15

	mov rax,9
	int 0x80

  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx

	leave
	ret

clear:
	push rbp
	mov rbp,rsp

  push rbx
  push rbp
  push r12
  push r13
  push r14
  push r15

	mov rax,11
	int 0x80

  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx

	leave
	ret

set_color:
	push rbp
	mov rbp,rsp

  push rbx
  push rbp
  push r12
  push r13
  push r14
  push r15

	mov rax,13
	int 0x80

  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx

	leave
	ret

set_back_color:
	push rbp
	mov rbp,rsp

  push rbx
  push rbp
  push r12
  push r13
  push r14
  push r15

	mov rax,14
	int 0x80

  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx

	leave
	ret
	
pixel:
push rbp
mov rbp,rsp
  push rbx
  push rbp
  push r12
  push r13
  push r14
  push r15
mov rax,10
int 0x80
  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx
leave
ret
