#include "syscall.h"
#include "proc.h"
#include "screen.h"
#include "types.h"

extern int sys_exit(void);
extern int sys_exec(void);
extern int sys_open(void);
extern int sys_close(void);
extern int sys_mknod(void);
extern int sys_write(void);
extern int sys_read(void);
extern int sys_fstat(void);

static int (*syscalls[])(void)={
		[SYS_exec]  sys_exec,
		[SYS_exit]  sys_exit,
		[SYS_open]  sys_open,
		[SYS_mknod] sys_mknod,
		[SYS_write] sys_write,
		[SYS_close] sys_close,
		[SYS_read] 	sys_read,
		[SYS_fstat] sys_fstat,
};
void dispatcher(int num,struct proc * proc){
	if(num >0 && num < (int)NELEM(syscalls) && syscalls[num] )
		proc->tf->eax=syscalls[num]();//保存返回值
	else{
			kprintf("unknow syscall\n");
			proc->tf->eax=-1;
	}
}
 static void syscall(registers_t * r  )
{
	struct proc* curproc=getcurproc();
	curproc->tf=r;
    int num =curproc->tf->eax;
//	kprintf("recv syscall: %d,%d\n",num,r->int_no);
	dispatcher(num,curproc);
	return ;
}

void init_syscall(void){
	register_interrupt_handler(64,syscall);

}


int getint(uint addr, int *ip)
{
  struct proc *curproc = getcurproc();

  if(addr >= curproc->sz || addr+4 > curproc->sz)
    return -1;
  *ip = *(int*)(addr);
  return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Doesn't actually copy the string - just sets *pp to point at it.
// Returns length of string, not including nul.
int getstr(uint addr, char **pp)
{
  char *s, *ep;
  struct proc *curproc = getcurproc();

  if(addr >= curproc->sz)
    return -1;
  *pp = (char*)addr;
  ep = (char*)curproc->sz;
  for(s = *pp; s < ep; s++){
    if(*s == 0)
      return s - *pp;
  }
  return -1;
}

// Fetch the nth 32-bit system call argument.
int
argint(int n, int *ip)
{
  return getint((getcurproc()->tf->esp) + 4 + 4*n, ip);
}

// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size bytes.  Check that the pointer
// lies within the process address space.
int
argptr(int n, char **pp, int size)
{
  int i;
  struct proc *curproc = getcurproc();
 
  if(argint(n, &i) < 0)
    return -1;
  if(size < 0 || (uint)i >= curproc->sz || (uint)i+size > curproc->sz)
    return -1;
  *pp = (char*)i;
  return 0;
}

// Fetch the nth word-sized system call argument as a string pointer.
// Check that the pointer is valid and the string is nul-terminated.
// (There is no shared writable memory, so the string can't change
// between this check and being used by the kernel.)
int
argstr(int n, char **pp)
{
  int addr;
  if(argint(n, &addr) < 0)
    return -1;
  return getstr(addr, pp);
}
