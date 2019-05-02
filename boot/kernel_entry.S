#include  "mmu.h"

.globl _start
_start=V2P_WO(entry)
.globl entry
entry :
  cli
  //打开 4M 页面扩展
  movl    %cr4, %eax
  orl     $(CR4_PSE), %eax
  movl    %eax, %cr4
  //设置页目录，在kernel.c 中定义
 movl    $(V2P_WO(entrypgdir)), %eax
 movl    %eax, %cr3
  //打开分页
  movl    %cr0, %eax
  orl     $(CR0_PG|CR0_WP), %eax
 movl    %eax, %cr0
  //debug
  # Set up the stack pointer.
  movl $(stack + KSTACKSIZE), %esp
  //movl  %esp, %ebp

  //间接跳转，完成到高地址的转移
  mov $kernel_main, %eax
  jmp *%eax
  
  .comm stack,KSTACKSIZE
