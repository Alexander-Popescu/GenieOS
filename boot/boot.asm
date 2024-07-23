[org 0x7c00]
[bits 16]

	mov bx, .string
	call printString

	jmp $

%include "printString.asm"

.string:
	db "Hello, World!"
	db 0

;boot sector padding
times 510-($-$$) db 0
dw 0xaa55
