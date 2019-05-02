#include "syscall.h"
#include "proc.h"
#include "screen.h"
 static void syscall(registers_t * r  )
{
	struct proc* curproc=getcurproc();
	curproc->tf=r;
    int num =curproc->tf->eax;
	kprintf("recv syscall: %d,%d\n",num,r->int_no);
	return ;
}

void init_syscall(void){
	register_interrupt_handler(64,syscall);

}
