#include "types.h"
#include "proc.h"
#include "kalloc.h"
#include "mmu.h"
#include "string.h"
#include "screen.h"
#include "vm.h"
#include "../cpu/isr.h"
#include "x86.h"
#include "fs.h"

struct {
	struct proc proc[NPROC];
}ptable;

static struct proc *initproc;
int nextpid=1;
extern void forkret(void);
extern void trapret(void);
extern void swtch(struct context **old, struct context *new);
extern void swtch1(struct context *new);
//static void wakeup1(void *chan);

//TODO: 当前进程
static struct proc *curproc=0;
struct proc * getcurproc(void){
	return curproc;
}

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

//调度
void scheduler(void)
{
  struct proc *p;
  
  for(;;){
    // 打开中断
    sti();

    // Loop over process table looking for process to run.
 //   acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;

      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
      switchuvm(p);
      p->state = RUNNING;
	  curproc=p;
//	  if(p->pid==1) swtch1(p->context); 
//	  else
      swtch(&curproc->context, p->context);
//      switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
    }
    //release(&ptable.lock);

  }
}
void uinit(void)
{
  struct proc *p;
  extern char _binary_init_start[], _binary_init_size[];

  p = allocproc();
  
  initproc = p;
  if((p->pgdir = setupkvm()) == 0)
    kprint("userinit: out of memory?");
  inituvm(p->pgdir, _binary_init_start, (int)_binary_init_size);
  p->sz = PGSIZE;
  memset(p->tf, 0, sizeof(*p->tf));
  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  p->tf->eflags = EFL_IF;
  p->tf->esp = PGSIZE;
  p->tf->eip = 0;  // beginning of init.S

  safestrcpy(p->name, "init", sizeof(p->name));
  p->cwd = namei("/");

  // this assignment to p->state lets other cores
  // run this process. the acquire forces the above
  // writes to be visible, and the lock is also needed
  // because the assignment might not be atomic.
  //acquire(&ptable.lock);

  p->state = RUNNABLE;

 // release(&ptable.lock);
  kprintf("initialized:get proc:%x ;pggir addr:%x\n",&p,&p->pgdir);
  isr_install();
  kprintf("set IDT  done!\nscheduler.....\n");
  scheduler();
}

void forkret(void){

  static int first = 1;

  if (first) {
    first = 0;
    iinit(ROOTDEV);
  }
		return ;
}
