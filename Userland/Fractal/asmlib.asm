GLOBAL pixel,sleep,malloc

pixel:
	push rbp
	mov rbp,rsp
	
	mov rax,10
	int 0x80
	
	leave
	ret

sleep:
	push rbp
	mov rbp,rsp
	
	mov rax,9
	int 0x80
	
	leave
	ret

malloc:
	push rbp
	mov rbp,rsp
	
	mov rax,8
	int 0x80
	
	leave
	ret
