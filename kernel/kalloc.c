// 物理内存分配器

#include "types.h"
#include "mmu.h"
#include "../libc/string.h"


void freerange(void *vstart, void *vend);
extern char end[]; // 内核加载结束后的第一个地址，在kernel.ld中定义

struct run {
  struct run *next;
};

struct {
  struct run *freelist;//空闲内存列表
} kmem;


void
kinit1(void *vstart, void *vend)
{
  freerange(vstart, vend);
}

void kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
}

//内存释放函数
void kfree(char *v)
{
  struct run *r;
  
  memset(v,1,PGSIZE);

  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
}

void freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}

// 内存分配函数
char*
kalloc(void)
{
  struct run *r;

  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  return (char*)r;
}

