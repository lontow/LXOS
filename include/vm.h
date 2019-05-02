#ifndef VM_H
#define VM_H
#include  "types.h"
#include "proc.h"
void kvmalloc(void);
void seginit(void);
void inituvm(pde_t * pgdir ,char *init,uint sz);
pde_t* setupkvm(void);
void switchuvm(struct proc *p);
#endif
