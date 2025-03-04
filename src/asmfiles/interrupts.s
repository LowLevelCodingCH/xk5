global swi_80_handler
extern swi_80
global defhandler
extern default_exception

section .text
swi_80_handler:
	cli
	pusha
	call swi_80
	popa
	sti
	iret
defhandler:
	cli
	pusha
	call default_exception
	popa
	sti
	iret
