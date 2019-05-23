
// 缓存层
//
// 


//
// 
//
//
//

#include "types.h"
#include "fs.h"
#include "bio.h"
#include "ide.h"
#include "screen.h"

struct {
  
  struct buf buf[NBUF];

  // 静态链表
  // 最近使用　head.next
  struct buf head;
} bcache;

void
binit(void)
{
  struct buf *b;

  


  // buf 链表创建
  bcache.head.prev = &bcache.head;
  bcache.head.next = &bcache.head;
  for(b = bcache.buf; b < bcache.buf+NBUF; b++){
    b->next = bcache.head.next;
    b->prev = &bcache.head;
   
    bcache.head.next->prev = b;
    bcache.head.next = b;
  }
}

//获取一个buf
static struct buf*
bget(uint dev, uint blockno)
{
  struct buf *b;

  //

  //该块是否已缓存
  for(b = bcache.head.next; b != &bcache.head; b = b->next){
    if(b->dev == dev && b->blockno == blockno){
      b->refcnt++;
    
      return b;
    }
  }

  // 
  for(b = bcache.head.prev; b != &bcache.head; b = b->prev){
    if(b->refcnt == 0 && (b->flags & B_DIRTY) == 0) {
      b->dev = dev;
      b->blockno = blockno;
      b->flags = 0;
      b->refcnt = 1;
     
      return b;
    }
  }
  kprint("bget: no buffers");
  return 0;
}

// 读取到buf
struct buf*
bread(uint dev, uint blockno)
{
  struct buf *b;

  b = bget(dev, blockno);
  if((b->flags & B_VALID) == 0) {
    iderw(b);
  }
  return b;
}

// 向磁盘写
void
bwrite(struct buf *b)
{
 // 
  //
  b->flags |= B_DIRTY;
  iderw(b);
}

// 
// 释放　buf并移动到最近用到的buf
void
brelse(struct buf *b)
{
  //
  //  

  //

  //
  b->refcnt--;
  if (b->refcnt == 0) {
    // 无人使用
    b->next->prev = b->prev;
    b->prev->next = b->next;
    b->next = bcache.head.next;
    b->prev = &bcache.head;
    bcache.head.next->prev = b;
    bcache.head.next = b;
  }
  

}


