#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // 如果参数个数不等于1，打印错误信息并退出
    if (argc != 1) {
        fprintf(2, "usage: pingpong\n");
        exit(1);
    }
    int p[2], c[2];
    char pbuf[1];
    pipe(p); // 创建管道，p[0]为读端，p[1]为写端，半双工通信
    pipe(c); // 创建管道，c[0]为读端，c[1]为写端，半双工通信
    if (fork() == 0) {  // 子进程
        // 子进程关闭管道 p 的写端，管道 c 的读端
        char cbuf[10];
        close(p[1]);
        close(c[0]);
        if (read(p[0], cbuf, 1) > 0) {
            fprintf(1, "%d: received ping\n", getpid());
            // sleep(1); // 模拟处理时间
        }
        // 向管道 c 写入数据
        write(c[1], "x", 1);
        // 关闭管道 c 的写端
        close(c[1]);
        // 关闭管道的读端
        close(p[0]);
        exit(0);
    } else {
        // 父进程关闭管道 p 的读端，管道 c 的写端
        close(p[0]);
        close(c[1]);
        write(p[1], "x", 1);
        if (read(c[0], pbuf, 1) > 0) {
            fprintf(1, "%d: received pong\n", getpid());
            // sleep(1); // 模拟处理时间
        }
        // 父进程关闭管道 p 的写端
        close(p[1]);
        // 关闭管道 c 的读端
        close(c[0]);
    }
    exit(0);
}