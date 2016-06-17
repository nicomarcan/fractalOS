GLOBAL loader
EXTERN main

loader:
	push rbp
	mov rbp,rsp
	call main
	leave
	ret

