#define SYS_exec 1
#define SYS_exit 2
#ifndef __ASSEMBLER__
#include "types.h"
void init_syscall(void);
int argint(int n,int *p);
int argstr(int n,char **pp);
int argptr(int n,char **pp,int size);
int getstr(uint addr, char **pp);
int getint(uint addr, int *ip);
#endif
