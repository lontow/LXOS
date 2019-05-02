#include "screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "types.h"
#include "mmu.h"
#include "kernel.h"
#include "kalloc.h"
#include  "vm.h"
#include  "x86.h"
#include "proc.h"
extern char end[];
__attribute__((__aligned__(PGSIZE)))
 pde_t entrypgdir[NPDENTRIES] ={
[0]=(0)|PTE_P|PTE_W|PTE_PS,
[512] =(0)|PTE_P|PTE_W|PTE_PS,
};


void kernel_main(){

		kinit1(end,P2V(4*1024*1024));
		kvmalloc();
		seginit();
		kinit2(P2V(4*1024*1024), P2V(PHYSTOP));
		uinit();
		clear_screen();
		kprint("Welcome to LTxOS!\n"
				"********************************************************\n");
	kprint("Type something ,it will go through the kernel\nEND to halt or PAGE to request a kmalloc()\n>");
		kprintf("Welcome %d,%c,%s,\n"
				"********************************************************\n",120,'x',"Lontow");
		/*
		kprint("Type something ,it will go through the kernel\nEND to halt or PAGE to request a kmalloc()\n>");
		
		clear_screen();
		
		isr_install();
		irq_install();	
		*/
/*		port_byte_out(0x3d4,14);
		int position = port_byte_in(0x3d5);
		position=position<<8;
		port_byte_out(0x3d4,15);
		position+=port_byte_in(0x3d5);

		int offset_from_vga =position*2;
		char *vga=(char*)0xb8000;
		vga[offset_from_vga]='X';
		vga[offset_from_vga+1]=0x0f;
*/
		
/*		kprint_at("X\n",6,1);
		kprint_at("This text spans mutiple lines\n",10,75);
		kprint_at("This is a line \n break\n",0,20);
		kprint("There is a line \n break\n");
		kprint_at("What happens when we run out of space?\n",45,24);
		*/
//		int i=0;
//		for(i=0;i<24;i++){
//				char str[255];
//				int_to_ascii(i,str);
//				kprint_at(str,0,i);
//		}
//		kprint_at("This text forces the kernel to scroll.",60,24);
//		kprint("This text forces the kernel to scroll,too.");

		
/*		
		__asm__ __volatile__("int $2");
		__asm__ __volatile__("int $3");
		__asm__ __volatile__("int $4");
		__asm__ __volatile__("int $5");
*/
		//asm volatile("sti");
		//init_timer(50);
		//
		//init_keyboard();
		
		


}







