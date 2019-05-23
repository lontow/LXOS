#ifndef FILE_H
#define FILE_H

#include "stat.h"
#define NDIRECT 12
#define NDEV         10  // 最大的设备数
#define NFILE       100  // 最大同时打开的文件数
#define NOFILE       16  // 每个文件能打开的最大文件数
struct file {
  enum { FD_NONE, FD_PIPE, FD_INODE } type;
  int ref; // 引用计数
  char readable;
  char writable;
  struct pipe *pipe;
  struct inode *ip;
  uint off;
};


//内存inode
struct inode {
  uint dev;           // 设备号
  uint inum;          // Inode 号
  int ref;            // 引用计数

  int valid;          // 是否有效

  short type;         // 硬盘inode 复制 
  short major;
  short minor;
  short nlink;
  uint size;
  uint addrs[NDIRECT+1];
};

// 设备数组
struct devsw {
  int (*read)(struct inode*, char*, int);
  int (*write)(struct inode*, char*, int);
};

extern struct devsw devsw[];

struct file*    filealloc(void);
void            fileclose(struct file*);
struct file*    filedup(struct file*);
void            fileinit(void);
int             fileread(struct file*, char*, int n);
int             filestat(struct file*, struct stat*);
int filewrite(struct file *f, char *addr, int n);
#define CONSOLE 1
#endif
