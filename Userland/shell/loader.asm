GLOBAL loader
EXTERN shell_main

section .text

loader:
	call shell_main
	ret
