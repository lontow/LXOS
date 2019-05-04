#ifndef VM_H
#define VM_H
#include  "types.h"
#include "proc.h"
#include "fs.h"
#define MAXARG 5
void kvmalloc(void);
void seginit(void);
void inituvm(pde_t * pgdir ,char *init,uint sz);
pde_t* setupkvm(void);
void switchuvm(struct proc *p);
void clearpteu(pde_t *pgdir, char *uva);
int loaduvm(pde_t *pgdir, char *addr, struct inode *ip, uint offset, uint sz);
int deallocuvm(pde_t *pgdir, uint oldsz, uint newsz);
int allocuvm(pde_t *pgdir, uint oldsz, uint newsz);
int copyout(pde_t *pgdir, uint va, void *p, uint len);
void freevm(pde_t *pgdir);
#endif
