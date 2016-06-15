GLOBAL read, write

section .text

write:
  push rbp
  mov rbp, rsp

  mov eax, 1
  int 80h
  leave
  ret
