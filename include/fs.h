#ifndef FS_H
#define FS_H
#include "file.h"
#include "stat.h"
//
//文件系统头文件


#define ROOTINO 1  // 根
#define BSIZE 512  // 块大小
#define ROOTDEV       1  // 根设备号

#define NINODE       50  // 最大活动　inode 数
// 硬盘布局:
// [ 引导 | 超级块 |  inode 块 |
//                                          空闲块bitmap映射 | 数据块]
//
// mkfs 计算sb 
struct superblock {
  uint size;         // 文件系统的总块数
  uint nblocks;      // 数据块数
  uint ninodes;      // inode 数
  uint nlog;         // log 层（未用到）
  uint logstart;     // 
  uint inodestart;   // 第一个inode 所属块
  uint bmapstart;    // 第一个空闲映射块号
};

#define NINDIRECT (BSIZE / sizeof(uint))
#define MAXFILE (NDIRECT + NINDIRECT)

// 硬盘　inode 结构
struct dinode {
  short type;           // 文件类型
  short major;          // 主设备号
  short minor;          // 次设备号
  short nlink;          // 链接数
  uint size;            // 文件大小
  uint addrs[NDIRECT+1];   // 数据块地址表
};

// 每块的　inode 数
#define IPB           (BSIZE / sizeof(struct dinode))

// 包含　inode i 的块
#define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)

// 每块的bitmap 位数
#define BPB           (BSIZE*8)

// 
#define BBLOCK(b, sb) (b/BPB + sb.bmapstart)

// 
#define DIRSIZ 14

struct dirent {
  ushort inum;
  char name[DIRSIZ];
};


#define MAXOPBLOCKS 	10
#define LOGSIZE 		(MAXOPBLOCKS*3)
#define NBUF 			(MAXOPBLOCKS*3)
#define FSSIZE  		1000


void            readsb(int dev, struct superblock *sb);
int             dirlink(struct inode*, char*, uint);
struct inode*   dirlookup(struct inode*, char*, uint*);
struct inode*   ialloc(uint, short);
struct inode*   idup(struct inode*);
void            iinit(int dev);
void            ilock(struct inode*);
void            iput(struct inode*);
void            iunlock(struct inode*);
void            iunlockput(struct inode*);
void            iupdate(struct inode*);
int             namecmp(const char*, const char*);
struct inode*   namei(char*);
struct inode*   nameiparent(char*, char*);
int             readi(struct inode*, char*, uint, uint);
void            stati(struct inode*, struct stat*);
int             writei(struct inode*, char*, uint, uint);

#endif
