	[org 0x7C00]

	mov bp, 0x8000 ; move stack far away
	mov sp, bp

	mov bx, 0x9000 ; ES is 0 by default
	mov dh, 2 ; read two sectors
	; dl is set automatically by the BIOS
	call diskLoad

	mov dx, [0x9000] ; first word of first sector
	call printHex

	call printNewLine

	mov dx, [0x9000 + 512] ; 1 sector out from first sector, first word of second sector
	call printHex

	jmp $

%include "printString.asm"
%include "printHex.asm"
%include "disk.asm"

	; identify sector as boot sector
	times 510 - ($-$$) db 0
	dw 0xAA55

	; we are past the boot sector now, information is considered on the hard
	; drive but it is not loaded into memory by the BIOS
	
	times 256 dw 0xABCD ; fill second sector with ABCD
	times 256 dw 0xFACE ; fill third sector with FACE
