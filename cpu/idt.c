//中断描述表

#include "idt.h"

//设置门，handler 处理函数地址
void set_idt_gate(int n,uint32_t handler){
		idt[n].low_offset=low_16(handler);
		idt[n].sel=KERNEL_CS;//gdt 代码段
		idt[n].always0=0;
		if(n==64) idt[n].flags=0xEF;
		else
		idt[n].flags=0x8E;
		idt[n].high_offset=high_16(handler);
}

//加载ldt
void set_idt(){
		idt_reg.base=(uint32_t)&idt;
		idt_reg.limit=IDT_ENTRIES*sizeof(idt_gate_t)-1;
		__asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
