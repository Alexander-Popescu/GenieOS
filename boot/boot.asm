[org 0x7c00]
[bits 16]

	mov [BOOT_DRIVE], dl

	;setup stack
	mov bp, 0x9000
	mov sp, bp

	mov bx, WELCOME_MESSAGE
	call printString
	mov bx, NEWLINE
	call printString

	mov bx, KERNEL_LOAD_MESSAGE
	call printString

	mov bx, KERNEL_OFFSET
	mov dh, 32
	mov dl, [BOOT_DRIVE]
	call loadKernel

	mov bx, NEWLINE
	call printString

	mov bx, ATTEMPT_MESSAGE
	call printString
	mov bx, NEWLINE
	call printString

	call enterProtectedMode

	jmp $

%include "disk.asm"
%include "printString.asm"
%include "enterProtectedMode.asm"
%include "setupGDT.asm"
%include "32BitPrint.asm"

[bits 32]
startProtectedMode:
	; update segmentation registers
	mov ax, DATA_SEGMENT
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; move stack
	mov ebp, 0x90000
	mov esp, ebp

	mov ebx, ENTER_MESSAGE
	call print_string_pm

	call KERNEL_OFFSET

	jmp $

WELCOME_MESSAGE: ; with newline and carriage return
	db "Welcome to GenieLoader, the official GenieOS Bootloader", 0xa, 0

KERNEL_LOAD_MESSAGE:
	db "Loading kernel...", 0

ATTEMPT_MESSAGE:
	db "Attempting to switch to 32-Bit Protected Mode...", 0

ENTER_MESSAGE:
	db "Entered 32Bit Protected Mode", 0

NEWLINE:
	db 0xa, 0xd, 0

SUCCESS_MESSAGE:
	db "SUCCESS", 0

BOOT_DRIVE:
	db 0

KERNEL_OFFSET equ 0x1000

;boot sector padding
times 510-($-$$) db 0
dw 0xaa55
