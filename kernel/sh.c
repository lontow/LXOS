#include "sh.h"
#include "../libc/string.h"
#include "screen.h"
#include "types.h"
void user_input(char * input){
		if(strcmp(input,"CLS")==0){
		clear_screen();
		}
		else if(strcmp(input,"END")==0){
				kprint("Stopping the CPU.Bye!\n");
				asm volatile("hlt");
		}else if(strcmp(input,"PAGE")==0){
				uint phys_addr=0;
				uint page = 0;//kmalloc(1000,1,&phys_addr);
				char page_str[16]="";
				hex_to_ascii(page,page_str);
				char phys_str[16]="";
				hex_to_ascii(phys_addr,phys_str);
				kprint("Page:");
				kprint(page_str);
				kprint(",physical address: ");
				kprint(phys_str);
				kprint("\n");
		}
		kprint("You said:");
		kprint(input);
		kprint("\n>");
}
