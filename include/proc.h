#ifndef PROC_H
#define PROC_H
#include "types.h"
#include "../cpu/isr.h"
#include "file.h"

#define NPROC 64  // 最大进程个数
//上下文环境
struct context{
	uint edi;
	uint esi;
	uint ebx;
	uint ebp;
	uint eip;

};

//进程状态
enum procstate {UNUSED,EMBYRO,SLEEPING,RUNNABLE,RUNNING};

//进程
struct proc{
	uint sz;
	pde_t* pgdir;
	char *kstack;
	enum procstate state;
	int pid;
	struct proc *parent;
	registers_t  *tf;
	struct context *context;
	void *chan;
	int killed;
	struct file *ofile[NOFILE];
	struct inode *cwd;
	char name[16];
};



void uinit(void);
struct proc * getcurproc(void);
void exit(void);
#endif
