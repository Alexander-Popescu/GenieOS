	[org 0x7C00]

	;some code to run the function
	mov bx, string
	call printString
	jmp $



	;print function that starts printing a string
	;at position specified by bx
printString:
	push ax
	push si
	push bx

	mov ah, 0x0E ; select bios function
	mov si, bx

.loop:
	lodsb ; loads si into al then increments si
	cmp al, 0 ; check for end of string
	jz .done
	int 0x10 ; print al to console
	jmp .loop

.done:
	pop bx
	pop si
	pop ax
	ret


string:
	db "Hello World!"
	db 0
	;for testing function

; create bootable file

	times 510 - ($-$$) db 0
	dw 0xAA55
	
