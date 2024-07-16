	[org 0x7C00]

	;some code to run the function

	mov bx, string
	call printString

	call printNewLine

	mov dx, 0xABCD
	call printHex
	jmp $

%include "printString.asm"
%include "printHex.asm"

string:
	;for testing function
	db "Print Hex test with 0xABCD:"
	db 0


	; identify sector as boot sector
	times 510 - ($-$$) db 0
	dw 0xAA55

	; we are past the boot sector now, information is considered on the hard
	; drive but it is not loaded into memory by the BIOS
	
	
