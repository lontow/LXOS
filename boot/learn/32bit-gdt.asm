gdt_start:

	dd 0x0
	dd 0x0

gdt_code:
	dw 0xffff ;limit 0-15
	dw 0x0    ;base 0-15
	db 0x0    ;base 16-23
	db 10011010b ;present,pri 00,d 1,type,1010
	db 11001111b ;gra 1,32-mode 1,64-mode 0,avl 0,limit 16-19
	db 0x0    ;base 24-31

gdt_data:
	dw 0xffff ;limit 0-15
	dw 0x0    ;base 0-15
	db 0x0    ;base 16-23
	db 10010010b ;present,pri 00,d 1,type,1010
	db 11001111b ;gra 1,32-mode 1,64-mode 0,avl 0,limit 16-19
	db 0x0    ;base 24-31

gdt_end:

gdt_descriptor:
	dw gdt_end-gdt_start-1
	dd gdt_start

CODE_SEG equ gdt_code - gdt_start

DATA_SEG equ gdt_data - gdt_start
