[org 0x7C00]

	mov bp, 0x9000 ; stack location
	mov sp, bp

	mov bx, REAL_MODE_MESSAGE
	call printString

	call switchProtected ; switch to 32 bit protected mode

	jmp $

%include "printString.asm"
%include "32bitPrint.asm"
%include "32bitGDT.asm"
%include "32bitSwitch.asm"

	;time for our actual code
[bits 32]
protectedStart:
	mov ebx, PROTECTED_MODE_MESSAGE
	call printStringProtected
	jmp $

REAL_MODE_MESSAGE db "Started in 16-bit real mode", 0
PROTECTED_MODE_MESSAGE db "Switched to 32-bit protected mode", 0

; identify sector as boot sector
times 510 - ($-$$) db 0
dw 0xAA55
