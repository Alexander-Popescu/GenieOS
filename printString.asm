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

printNewLine:
    pusha
    
    mov ah, 0x0e
    mov al, 0x0a ; newline char
    int 0x10
    mov al, 0x0d ; carriage return
    int 0x10
    
    popa
    ret
