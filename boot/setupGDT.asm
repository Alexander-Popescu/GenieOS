gdt_start
null_descriptor:
	dd 0
	dd 0
code_descriptor:
	dw 0xffff    ; segment length, bits 0-15
	dw 0x0       ; segment base, bits 0-15
	db 0x0       ; segment base, bits 16-23
	db 10011010b ; flags (8 bits)
	db 11001111b ; flags (4 bits) + segment length, bits 16-19
	db 0x0       ; segment base, bits 24-31	data_descriptor
data_descriptor:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:	

GDT_Descriptor:
	dw gdt_end - gdt_start - 1 ; size of gdt
	dd gdt_start ; start pointer

CODE_SEGMENT equ code_descriptor - gdt_start
DATA_SEGMENT equ data_descriptor - gdt_start
