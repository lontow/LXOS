
#include "syscall.h"
#include "traps.h"

#define SYSCALL(name) \
  .globl name; \
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(exit)
SYSCALL(exec)
SYSCALL(open)
SYSCALL(close)
SYSCALL(mknod)
SYSCALL(write)
SYSCALL(read)
