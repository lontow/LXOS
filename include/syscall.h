#define SYS_exec 1
#define SYS_exit 2
#define SYS_open 3 
#define SYS_mknod 4
#define SYS_write 5 
#define SYS_read 6 
#define SYS_close 7 
#ifndef __ASSEMBLER__
#include "types.h"
void init_syscall(void);
int argint(int n,int *p);
int argstr(int n,char **pp);
int argptr(int n,char **pp,int size);
int getstr(uint addr, char **pp);
int getint(uint addr, int *ip);
#endif
