
//init:第一个用户层程序 

#include "types.h"
#include "stat.h"
#include "screen.h"
#include "user.h"
#include "fctl.h"

char *argv[] = { "sh", 0 };
char buf[8192];

int stat(const char * path,struct stat *st)
{
		int fd=open(path,O_RDONLY);
		  printf(0,"fs.text 's fd is:%d\n",fd);
		if(fd <=0) return -1;
		int r=fstat(fd,st);
		close(fd);
		return r;
}
int main(void)
{
  //int pid, wpid;
  if(open("console", O_RDWR) < 0){
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
//  dup(0);  // 0
//  dup(0);  // stderr

 // for(;;){
    printf(0,"init: starting sh\n");
    printf(0,"test create file\n");

  int fd;
  int i;

  fd = open("small", O_CREATE|O_RDWR);
  if(fd >= 0){
    printf(0, "creat small succeeded; ok\n");
  } else {
    printf(0, "error: creat small failed!\n");
    //exit();
  }
  for(i = 0; i < 2; i++){
    if(write(fd, "aaaaaaaaaa", 10) != 10){
      printf(0, "error: write aa %d new file failed\n", i);
    //  exit();
    }
    if(write(fd, "bbbbbbbbbb", 10) != 10){
      printf(0, "error: write bb %d new file failed\n", i);
     // exit();
    }
  }
  printf(0, "writes ok\n");
  close(fd);
  struct stat st;
  st.size=7;
  if(stat("fs.txt",&st))
		  printf(0,"fs.text size is:%d\n",st.size);
  else{
  //		printf(0,"get fs.txt stat,failed!\n");
//		exit();
  }
  fd = open("fs.txt", O_RDONLY);
  if(fd >= 0){
    printf(0, "open fs.txt succeeded ok\n");
  } else {
    printf(0, "error: open small failed!\n");
   // exit();
  }
  i = read(fd, buf, sizeof(buf));
  if(i ){
    printf(0, "read succeeded ok\n");

	printf(0,"content:%s\n",buf);
  } else {
    printf(0, "read failed\n");
    //exit();
  }
  close(fd);

//exit();
   // pid = fork();
   // if(pid < 0){
    //  printf(1, "init: fork failed\n");
    //  exit();
   // }
   // if(pid == 0){
    //  exec("sh", argv);
    //  printf(1, "init: exec sh failed\n");
    //  exit();
    //}
//    while((wpid=wait()) >= 0 && wpid != pid)
  //    printf(1, "zombie!\n");
  //}
  return 0;
}
