	; prints the hexadecimal word in dx
printHex:
	pusha
	mov ah, 0x0E ; select bios function
	mov cx, 4 ; index

.loop:
	mov ax, dx
	and ax, 0x000F ; get last nibble
	add al, 0x30 ; convert to ASCII
	cmp al, 0x39 ; if > 9 add extra 8 for A to F
	jle .else
	add al, 7 ; proper ASCII conversion
.else:
	; now the ASCII value is in al, we need to move it to the right spot
	; in the output block

	;calculate address in bx
	mov bx, .output + 1
	add bx, cx
	mov [bx], al ; move to address
	ror dx, 4 ; rotate 4 bits to have next character in the last digit



	sub cx, 1
	cmp cx, 0 ; check if all 4 characters were moved into output
	je .done
	jmp .loop
	
.done:
	
	;use print function to print output
	mov bx, .output
	call printString
	
	popa
	ret


.output:
	db '0x0000',0

