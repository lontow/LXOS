[extern bootmain]


[bits 16]
bootstart:
	cli
	mov [BOOT_DRIVE],dl
	mov bp,0x9000
	mov sp,bp

	;mov bx,MSG_REAL_MODE

	;call print
	
 	call  check_a20
 	cmp al,0x01
 	jne stage2
 	
 	;mov bx ,A20
   ; call print
stage2:
	
	call switch_to_pm
	jmp $

;%include "boot/boot_sect_print.asm"
;%include "boot/boot_sect_print_hex.asm"


check_a20:
	pushf
	push ds
	push si
	xor ax,ax
	not ax
	mov ds,ax
	mov si,0x7E0E
   cmp byte [ds:si], 0xaa
    mov al, 0x0
    je check_a20__exit
    mov al, 0x1
check_a20__exit:
	pop si
 	pop ds
	popf
    ret

switch_to_pm:
	lgdt [gdt_descriptor]
	mov eax,cr0
	or eax,0x1
	mov cr0,eax
	jmp CODE_SEG:init_pm

[bits 32]

%include "boot/32bit-print.asm"
init_pm:
	mov ax,DATA_SEG
	mov ds,ax
	mov ss,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

	mov esp,bootstart
;start PM
	mov ebx,MSG_LOAD_KERNEL
	call print_string_pm
	call bootmain
	jmp $

;GDT 
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

;MSG_REAL_MODE:
;	db "Started in 16-bit real mode",0

;MSG_PROT_MODE:

;	db "Loaded 32-bit protected mode",0

BOOT_DRIVE:
	db 0
MSG_LOAD_KERNEL:

	db "Loading kernel into memory",0
;MSG_RETURNED_KERNEL db "Returned from kernel.Error?",0

;A20:
;	db "A20 enabled",0

;times 510-($-$$) db 0
;dw 0xaa55

