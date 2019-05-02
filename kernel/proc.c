#include "types.h"
#include "proc.h"
#include "kalloc.h"
#include "mmu.h"
#include "../libc/string.h"
#include "screen.h"
#include "vm.h"
#include "../cpu/isr.h"

struct {
	struct proc proc[NPROC];
}ptable;

static struct proc *initproc;
int nextpid=1;
extern void forkret(void);
extern void trapret(void);
//static void wakeup1(void *chan);

//TODO: 当前进程

//寻找一个 UNUSED 状态的进程，进行初始化 EMBYRO
static struct proc* allocproc(void){
	struct proc *p;
	char *sp;
	int i=0;
	for(p=&ptable.proc[i];i<NPROC;i++){
		if(p->state==UNUSED){
			p->state =EMBYRO;
			p->pid=nextpid++;
			if((p->kstack=kalloc())==0){
				p->state= UNUSED;
				return 0;
			}
			
			sp =p->kstack + KSTACKSIZE;
			sp -= sizeof(*p->tf);
			p->tf=(registers_t*)sp;
			sp-=4;
			*(uint*)sp=(uint)trapret;
			 sp -= sizeof *p->context;
  			p->context = (struct context*)sp;
  			memset(p->context, 0, sizeof *p->context);
  			p->context->eip = (uint)forkret;

  			return p;
	}
	}
	
	return 0;
}

void
uinit(void)
{
  struct proc *p;
  extern char _binary_init_start[], _binary_init_size[];

  p = allocproc();
  
  initproc = p;
  if((p->pgdir = setupkvm()) == 0)
    kprint("userinit: out of memory?");
  inituvm(p->pgdir, _binary_init_start, (int)_binary_init_size);
  p->sz = PGSIZE;
  kprintf("get p:\naddr:%x\n",&p,&p->pgdir);
  memset(p->tf, 0, sizeof(*p->tf));
  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  //p->tf->eflags = FL_IF;
  p->tf->esp = PGSIZE;
  p->tf->eip = 0;  // beginning of init.S

 // safestrcpy(p->name, "init", sizeof(p->name));
 // p->cwd = namei("/");

  // this assignment to p->state lets other cores
  // run this process. the acquire forces the above
  // writes to be visible, and the lock is also needed
  // because the assignment might not be atomic.
  //acquire(&ptable.lock);

  p->state = RUNNABLE;

 // release(&ptable.lock);
}

void forkret(void){
		return ;
}
