; Reads DH sectors from drive DL into ES:BX, starting at CHS 0/0/2
; Handles track and head boundaries (18 sectors/track, 2 heads)
loadKernel:
    pusha
    mov cl, 2           ; starting sector
    xor ch, ch          ; cylinder 0
    mov [dk_n], dh      ; save sector count
    mov [dk_drv], dl    ; save drive number
    xor dh, dh          ; head 0
.read:
    cmp byte [dk_n], 0
    je .done
    mov dl, [dk_drv]    ; restore drive number (INT 0x13 can clobber)
    mov al, 1           ; read 1 sector
    mov ah, 0x02
    int 0x13
    jc disk_error
    dec byte [dk_n]
    add bx, 0x200
    inc cl
    cmp cl, 19          ; sectors 1-18 per track
    jb .read
    mov cl, 1
    xor dh, 1           ; toggle head
    test dh, dh
    jnz .read
    inc ch              ; next cylinder when head wraps to 0
    jmp .read
.done:
    popa
    ret

disk_error:
    mov bx, DISK_ERROR
    call printString
    jmp $

dk_n:   db 0
dk_drv: db 0
DISK_ERROR: db "Disk read error", 0
