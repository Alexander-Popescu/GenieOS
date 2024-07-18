	; function loads 'dh' sectors from drive 'dl' into ES:BX
diskLoad:
	pusha 
	push dx

	mov ah, 0x02 ; BIOS setting for read 
	mov al, dh ; number of sectors to read stored in al
	mov cl, 0x02 ; first non MBR sector, sector goes here
	mov ch, 0x00
	; dl is drive number, set as a parameter
	mov dh, 0x00 ; head number

	int 0x13 ; BIOS interrupt
	jc diskError ; carry bit stores error

	pop dx
	cmp al, dh ; BIOS sets al to number of sectors read, so compare
	jne sectorsError


	popa
	ret

diskError:
	mov bx, DISK_ERROR
	call printString
	call printNewLine
	mov dh, ah ; dl already has disk drive num and ah has error code
	call printHex
	jmp diskLoop

sectorsError:
	mov bx, SECTORS_ERROR
	call printString

diskLoop:
	jmp $

DISK_ERROR: db "Disk Read Error", 0
SECTORS_ERROR: db "Incorrect Number of Sectors Read", 0
