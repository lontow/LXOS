//用于加载内核

#include "types.h"
#include "x86.h"
#include "elf.h"

#define SECTSIZE  512

void readseg(uchar*, uint, uint);

void
bootmain(void)
{
  struct elfhdr *elf;//elf 文件头
  struct proghdr *ph, *eph;//程序段的头
  void (*entry)(void);//入口函数指针
  uchar* pa;

  elf = (struct elfhdr*)0x10000;  

  // 读取前 4096个字节
  readseg((uchar*)elf, 4096, 0);

  // 判断是不是一个可执行的ELF 文件
  if(elf->magic != ELF_MAGIC)
    return;  

  // 加载程序段
  ph = (struct proghdr*)((uchar*)elf + elf->phoff);
  eph = ph + elf->phnum;
  for(; ph < eph; ph++){
    pa = (uchar*)ph->paddr;
    readseg(pa, ph->filesz, ph->off);
    if(ph->memsz > ph->filesz)
      stosb(pa + ph->filesz, 0, ph->memsz - ph->filesz);
  }

  // 内核入口函数
  entry = (void(*)(void))(elf->entry);
  entry();
}

/*
内核读取

1F0        - 数据寄存器。读写数据都必须通过这个寄存器

1F1        - 错误寄存器，每一位代表一类错误。全零表示操作成功。

1F2        - 扇区计数。这里面存放你要操作的扇区数量

1F3        - 扇区LBA地址的0-7位

1F4        - 扇区LBA地址的8-15位

1F5        - 扇区LBA地址的16-23位

1F6 (低4位) - 扇区LBA地址的24-27位

1F6 (第4位) - 0表示选择主盘，1表示选择从盘

1F6 (5-7位) - 必须为1

1F7 (写)    - 命令寄存器

1F7 (读)    - 状态寄存器

              bit 7 = 1  控制器忙
              bit 6 = 1  驱动器就绪
              bit 5 = 1  设备错误
              bit 4        N/A
              bit 3 = 1  扇区缓冲区错误
              bit 2 = 1  磁盘已被读校验
              bit 1        N/A
              bit 0 = 1  上一次命令执行失败
*/

void
waitdisk(void)
{
  // 等待硬盘
  while((inb(0x1F7) & 0xC0) != 0x40)
    ;
}

// 读一个扇区
void readsect(void *dst, uint offset)
{
  // 发送命令
  waitdisk();
  outb(0x1F2, 1);   // 一个扇区
  outb(0x1F3, offset);
  outb(0x1F4, offset >> 8);
  outb(0x1F5, offset >> 16);
  outb(0x1F6, (offset >> 24) | 0xE0);
  outb(0x1F7, 0x20);  // cmd 0x20 - read sectors

  // 读数据
  waitdisk();
  insl(0x1F0, dst, SECTSIZE/4);
}

// 从 距内核 offset 处读取 count个字节dao物理地址 pa
void
readseg(uchar* pa, uint count, uint offset)
{
  uchar* epa;

  epa = pa + count;

  // 扇区对齐
  pa -= offset % SECTSIZE;

  // 转为扇区数 ,跳过第一个扇区（用于引导）
     offset = (offset / SECTSIZE) + 1;

  for(; pa < epa; pa += SECTSIZE, offset++)
    readsect(pa, offset);
}
