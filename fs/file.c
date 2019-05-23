
//
// 文件描述符
//

#include "types.h"
#include "fs.h"

#include "file.h"
#include "screen.h"

struct devsw devsw[NDEV];
struct {
  struct file file[NFILE];
} ftable;

void
fileinit(void)
{
}

// 分配 file 文件结构
struct file*
filealloc(void)
{
  struct file *f;

  
  for(f = ftable.file; f < ftable.file + NFILE; f++){
    if(f->ref == 0){
      f->ref = 1;
    
      return f;
    }
  }
  
  return 0;
}

// 增加文件引用.
struct file*
filedup(struct file *f)
{
  //
    //
  f->ref++;
  //
  return f;
}

// 关闭文件
void
fileclose(struct file *f)
{
  struct file ff;

//  
  //i
  if(--f->ref > 0){
   // 
    return;
  }
  ff = *f;
  f->ref = 0;
  f->type = FD_NONE;
  //

 // 
   if(ff.type == FD_INODE){
    //
    iput(ff.ip);
    //
  }
}

// 获取文件状态
int
filestat(struct file *f, struct stat *st)
{
  if(f->type == FD_INODE){
    ilock(f->ip);
    stati(f->ip, st);
    iunlock(f->ip);
    return 0;
  }
  return -1;
}

// 读文件
int
fileread(struct file *f, char *addr, int n)
{
  int r;

  if(f->readable == 0)
    return -1;
  //
  if(f->type == FD_INODE){
    ilock(f->ip);
    if((r = readi(f->ip, addr, f->off, n)) > 0)
      f->off += r;
    iunlock(f->ip);
    return r;
  }
  kprint("fileread");
  return -1;
}

//
// 写文件
int
filewrite(struct file *f, char *addr, int n)
{
  int r;

  if(f->writable == 0)
    return -1;
 // 
  if(f->type == FD_INODE){
    // 控制写入数量
    int max = ((MAXOPBLOCKS-1-1-2) / 2) * 512;
    int i = 0;
    while(i < n){
      int n1 = n - i;
      if(n1 > max)
        n1 = max;

     // 
      ilock(f->ip);
      if ((r = writei(f->ip, addr + i, f->off, n1)) > 0)
        f->off += r;
      iunlock(f->ip);
      //

      if(r < 0)
        break;
      if(r != n1)
        kprint("short filewrite");
      i += r;
    }
    return i == n ? n : -1;
  }
  kprint("filewrite");
  return -1;
}

