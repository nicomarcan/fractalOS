GLOBAL read, write, time, sleep, memory, clear, set_color, set_back_color, pixel,free_mem,fkexec,_hlt,mem_realloc,exit,yield,ps,kill,mutex_lock,mutex_unlock,_wait

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

 _wait:

  pushrg
  
  mov rax, 3
  int 80h
  
  poprg
  ret

 mutex_lock:

  pushrg
  
  mov rax, 16
  int 80h
  
  poprg
  ret
  
 mutex_unlock:
  pushrg
  
  mov rax, 17
  int 80h
  
  poprg
  ret
	
 kill:
  pushrg

  mov rax, 15
  int 80h

  poprg
  ret
  
 mem_realloc:
  pushrg

  mov rax, 6
  int 80h

  poprg
  ret
  
 ps:
  pushrg

  mov rax, 2
  int 80h

  poprg
  ret
  
 exit:
  pushrg

  mov rax, 4
  int 80h

  poprg
  ret		
 
 fkexec:
  pushrg

  mov rax, 5
  int 80h

  poprg
  ret	

memory:
  pushrg

  mov rax, 8
  int 80h

  poprg
  ret
  
 free_mem:
  pushrg

  mov rax, 7
  int 80h

  poprg
  ret


write:
  pushrg

  mov rax, 1
  int 80h

  poprg
  ret

read:
  pushrg

  mov rax, 0
  int 80h

  poprg
  ret

time:
  pushrg

  mov rax, 12
  int 80h

  poprg
  ret

sleep:
  pushrg

  mov rax,9
  int 0x80

  poprg
  ret

clear:
  pushrg

  mov rax,11
  int 0x80

  poprg
  ret

set_color:
  pushrg

  mov rax,13
  int 0x80

  poprg
  ret

set_back_color:
  pushrg

  mov rax,14
  int 0x80

  poprg
  ret
	
pixel:
  pushrg
  
  mov rax,10
  int 0x80
  
  poprg
  ret
