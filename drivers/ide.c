
// 简单的IDE 驱动

#include "types.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "fs.h"
#include "buf.h"
#include "screen.h"
#include "function.h"
#include "../cpu/isr.h"

#define SECTOR_SIZE   512
#define IDE_BSY       0x80
#define IDE_DRDY      0x40
#define IDE_DF        0x20
#define IDE_ERR       0x01

#define IDE_CMD_READ  0x20
#define IDE_CMD_WRITE 0x30
#define IDE_CMD_RDMUL 0xc4
#define IDE_CMD_WRMUL 0xc5





static struct buf *idequeue;

static int havedisk1;
static void idestart(struct buf*);

// 等待ＩＤＥ　硬盘就绪
static int
idewait(int checkerr)
{
  int r;

  while(((r = inb(0x1f7)) & (IDE_BSY|IDE_DRDY)) != IDE_DRDY)
    ;
  if(checkerr && (r & (IDE_DF|IDE_ERR)) != 0)
    return -1;
  return 0;
}

void
ideinit(void)
{
  int i;


  idewait(0);

  // 检查disk 1
  outb(0x1f6, 0xe0 | (1<<4));
  for(i=0; i<1000; i++){
    if(inb(0x1f7) != 0){
      havedisk1 = 1;
	  kprint("dectected disk1\n");
      break;
    }
  }

  // 切换回disk 0.
  outb(0x1f6, 0xe0 | (0<<4));
}

// 实际读写函数
static void
idestart(struct buf *b)
{
  if(b == 0)
    kprint("idestart");
  if(b->blockno >= FSSIZE)
    kprint("incorrect blockno");
  int sector_per_block =  BSIZE/SECTOR_SIZE;
  int sector = b->blockno * sector_per_block;
  int read_cmd = (sector_per_block == 1) ? IDE_CMD_READ :  IDE_CMD_RDMUL;
  int write_cmd = (sector_per_block == 1) ? IDE_CMD_WRITE : IDE_CMD_WRMUL;

  if (sector_per_block > 7) kprint("idestart");

  idewait(0);
  outb(0x3f6, 0);  // 产生中断
  outb(0x1f2, sector_per_block);  // 读取的页面数
  outb(0x1f3, sector & 0xff);
  outb(0x1f4, (sector >> 8) & 0xff);
  outb(0x1f5, (sector >> 16) & 0xff);
  outb(0x1f6, 0xe0 | ((b->dev&1)<<4) | ((sector>>24)&0x0f));
  if(b->flags & B_DIRTY){
    outb(0x1f7, write_cmd);
    outsl(0x1f0, b->data, BSIZE/4);
  } else {
    outb(0x1f7, read_cmd);
  }
}

//中断处理函数
static void ideint_callback(registers_t *r)
{

 UNUSED(r);
  struct buf *b;



  if((b = idequeue) == 0){

    return;
  }
  idequeue = b->qnext;


  if(!(b->flags & B_DIRTY) && idewait(1) >= 0)
    insl(0x1f0, b->data, BSIZE/4);


  b->flags |= B_VALID;
  b->flags &= ~B_DIRTY;


  
  if(idequeue != 0)
    idestart(idequeue);

 
}

//注册中断函数
void ide_register(void){
		register_interrupt_handler(IRQ14,ideint_callback);
}


// 同步磁盘和　buf

void
iderw(struct buf *b)
{
  struct buf **pp;

 
  if((b->flags & (B_VALID|B_DIRTY)) == B_VALID)
    kprint("iderw: nothing to do");
  if(b->dev != 0 && !havedisk1)
    kprint("iderw: ide disk 1 not present");

 
  // 队列添加
  b->qnext = 0;
  for(pp=&idequeue; *pp; pp=&(*pp)->qnext)  
    ;
  *pp = b;

  // 必要时启动读写
  if(idequeue == b)
    idestart(b);

  // 等待操作完成
  while((b->flags & (B_VALID|B_DIRTY)) != B_VALID){

  }



}
