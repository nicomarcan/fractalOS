GLOBAL read, write

section .text

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
