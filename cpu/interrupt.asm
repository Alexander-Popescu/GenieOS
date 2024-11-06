;c function
[extern ISRHandler]
[extern IRQHandler]

isr_common_stub:
	;push information to the stack
	pusha
	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	;OS handle
	call ISRHandler
	pop eax 
	;return
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	;error code and interrupt number
	add esp, 8
	sti
	iret

irq_common_stub:
    pusha 
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call IRQHandler
    pop ebx

    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
    iret 

;macro for generating ISR functions for the IDT to point to
%macro ISR 1
;reference from c code
global isr%1



isr%1:
    cli
    %if %1 = 8 || %1 = 10 || %1 = 11 || %1 = 12 || %1 = 13 || %1 = 14 || %1 == 17
	;these interrupts automatically push an error code
        push %1
    %else
	;placeholder error code for consistency
        push 0
        push %1
    %endif
    jmp isr_common_stub
%endmacro

;generate all interrupts
%assign i 0
%rep 256
ISR i
%assign i i+1
%endrep

%macro IRQ 1

global irq%1

irq%1:
	cli
	push byte %1
	push byte 32 + i
	jmp irq_common_stub

%endmacro

%assign i 0
%rep 16
IRQ i
%assign i i+1
%endrep
