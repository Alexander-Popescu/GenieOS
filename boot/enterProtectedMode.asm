enterProtectedMode:
	cli ; disable interrupts
	lgdt [GDT_Descriptor] ; set GDT
	; change last bit of cr0 to 1
	mov eax, cr0
	or eax, 1
	mov cr0, eax ; 32 Bit mode active

	jmp 0x08:startProtectedMode

	ret
