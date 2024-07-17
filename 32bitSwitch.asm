[bits 16]
switchProtected:
    ; procedure for switching into 32 bit protected mode

    cli ; disable interrupts
    lgdt [gdt_descriptor] ; load the gdt descriptor

    ; actually tell the cpu that we want to move to 32 bit mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEGMENT:initProtectedMode

[bits 32]
initProtectedMode:
    ; update segmentation registers with new segment information
    mov ax, DATA_SEGMENT
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; new stack
    mov esp, ebp

    call protectedStart
