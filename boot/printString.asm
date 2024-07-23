;assumes string to print begins at bx and ends with null byte

printString:
	pusha
	;write character video service
	mov ah, 0x0e
	
	jmp .printStringLoop
.printStringDone:

	popa
	ret

.printStringLoop:
	mov al, [bx]
	cmp al, 0x0
	je .printStringDone
	int 0x10; print char
	add bx, 1
	jmp .printStringLoop
