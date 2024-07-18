[org 0x7C00]
KERNEL_OFFSET equ 0x1000 ; load kernel after bootloader but before other memory maps

	mov [BOOT_DRIVE], dl ; store in case dl is changed

	mov bp, 0x9000 ; stack location
	mov sp, bp

	mov bx, REAL_MODE_MESSAGE
	call printString
	call printNewLine

	call loadKernel
	call switchProtected ; switch to 32 bit protected mode

	jmp $

%include "boot/disk.asm"
%include "boot/printHex.asm"
%include "boot/printString.asm"
%include "boot/32bitPrint.asm"
%include "boot/32bitGDT.asm"
%include "boot/32bitSwitch.asm"

[bits 16]
loadKernel:
	mov bx, KERNEL_LOAD_MESSAGE
	call printString
	call printNewLine

	mov bx, KERNEL_OFFSET ; put read data from disk into 0x1000
	mov dh, 2
	mov dl, [BOOT_DRIVE]
	call diskLoad
	ret

	;time for our actual code
[bits 32]
protectedStart:
	mov ebx, PROTECTED_MODE_MESSAGE
	call printStringProtected
	call KERNEL_OFFSET
	jmp $

REAL_MODE_MESSAGE db "Started in 16-bit real mode", 0
PROTECTED_MODE_MESSAGE db "Switched to 32-bit protected mode", 0
KERNEL_LOAD_MESSAGE db "Loading Kernel into memory", 0
BOOT_DRIVE db 0 ;dl could be overwritten, so store it here

; identify sector as boot sector and pad sector
times 510 - ($-$$) db 0
dw 0xAA55
