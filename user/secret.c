#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"


int
main(int argc, char *argv[])
{
  if(argc != 2){
    printf("Usage: secret the-secret\n");
    exit(1);
  }
  // sbrk函数是一种在进程地址空间中调整堆空间大小的系统调用，它通过移动堆的边界来增加或减少可用内存的大小。
  // 这个函数通常用于实现动态内存分配函数，如malloc和free，是内存管理中的一个重要工具。
  char *end = sbrk(PGSIZE*32);
  end = end + 9 * PGSIZE;
  strcpy(end, "my very very very secret pw is:   ");
  strcpy(end+32, argv[1]);
  exit(0);
}

