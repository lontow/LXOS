#include "sh.h"
#include "../libc/string.h"
#include "screen.h"
#include "types.h"
//test ide driver
#include "ide.h"
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
		}else if(strcmp(input,"TESTIDEW")==0){
		
				struct buf buf1;
				buf1.blockno=1;
				buf1.flags=B_DIRTY;
				buf1.data[0]='e';
				buf1.data[1]='e';
				buf1.data[2]='e';
				buf1.data[3]='e';
				buf1.data[4]='e';
				buf1.data[5]='e';
				buf1.dev=1;
				iderw(&buf1);
				kprintf("ide write:%s\n","eeeeee");

		}else if(strcmp(input,"TESTIDER")==0){


				struct buf buf1;
				buf1.blockno=1;
				buf1.flags=B_DIRTY;
				buf1.dev=1;
				iderw(&buf1);
				kprintf("ide read:%c\n",buf1.data[0]);
		}
		kprint("You said:");
		kprint(input);
		kprint("\n>");
}
