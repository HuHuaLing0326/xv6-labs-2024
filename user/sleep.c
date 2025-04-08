#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  // 如果参数个数不等于2，打印错误信息并退出 
  if (argc != 2) {
    fprintf(2, "usage: sleep n\n");
    exit(1);
  }
  int n = atoi(argv[1]);
  // 如果参数小于0，打印错误信息并退出
  if (n < 0) {
    fprintf(2, "sleep: invalid time\n");
    exit(1);
  }
  sleep(n);
  exit(0);
}