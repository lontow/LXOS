#ifndef BUF_H
#define BUF_H
#include "types.h"
struct buf {
  int flags;
  uint dev;
  uint blockno;

  uint refcnt;
  struct buf *prev; // LRU 缓存链表
  struct buf *next;
  struct buf *qnext; // 硬盘队列
  uchar data[512];
};
#define B_VALID 0x2  // 缓存已从硬盘更新
#define B_DIRTY 0x4  // 缓存需要向硬盘更新

#endif
