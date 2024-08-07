;c function
[extern ISRHandler]

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
	
	;return
	pop eax 
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	;error code and interrupt number
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
