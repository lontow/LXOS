#ifndef STAT_H
#define STAT_H
#define T_DIR  1   // 目录
#define T_FILE 2   // 文件
#define T_DEV  3   // 设备

struct stat {
  short type;  // 文件类型
  int dev;     // 文件系统所属设备
  uint ino;    // Inode 号
  short nlink; // 链接数
  uint size;   // 文件大小
};
#endif
