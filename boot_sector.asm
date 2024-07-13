mov ah, 0x0e ; signal TTY mode
mov al, 'G'
int 0x10 ;video interrupt, writes al to tty
mov al, 'e'
int 0x10
mov al, 'n'
int 0x10
mov al, 'i'
int 0x10
mov al, 'e'
int 0x10
mov al, 'O'
int 0x10
mov al, 'S'
int 0x10

jmp $ ;infinite loop

; end of boot sector
times 510 - ($-$$) db 0
dw 0xaa55
