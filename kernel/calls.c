#include "../libc/string.h"
#include "types.h"
#include "syscall.h"
#include "screen.h"
#include "sh.h"

int sys_exec(void)
{

  char *path, *argv[5];
  int i;
  uint uargv, uarg;

  if(argstr(0, &path) < 0 || argint(1, (int*)&uargv) < 0){
    return -1;
  }
  memset(argv, 0, sizeof(argv));
  for(i=0;; i++){
    if(i >= (int)NELEM(argv))
      return -1;
    if(getint(uargv+4*i, (int*)&uarg) < 0)
      return -1;
    if(uarg == 0){
      argv[i] = 0;
      break;
    }
    if(getstr(uarg, &argv[i]) < 0)
      return -1;
  }
  kprintf("exec:%s,%s\n",path,argv[0]);
  user_input("test IDE");
  return 0;
}

int sys_exit(void){
		kprintf("exit\n");
		return 0;
}
