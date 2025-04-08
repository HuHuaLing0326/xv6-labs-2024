#include "kernel/types.h"
#include "user/user.h"

int 
primes_process(int fd)
{
    int prime_num;
    if (read(fd, &prime_num, sizeof(prime_num)) <= 0) {
        return 0;
    }
    if (prime_num== -1) {
        return 0;
    }
    fprintf(1, "prime %d\n", prime_num);

    int p[2];
    pipe(p);
    int pid = fork();
    if (pid == 0) {
        close(p[1]);
        close(fd);

        primes_process(p[0]);
        
        close(p[0]);
        exit(0);
    } else {
        close(p[0]);
        int buf;

        while(read(fd, &buf, sizeof(buf)) && buf != -1) {	
            if (buf % prime_num != 0) {			// 剔除素数的倍数
                write(p[1], &buf, sizeof(buf));
            }
        }
        close(p[1]);

        wait(0);
        exit(0);
    }
}

int
main(int argc, char*argv[])
{
    int i;
    int p[2];
    pipe(p);

    int pid = fork();
    if (pid == 0) {     // child
        close(p[1]);    // 只需读，关闭 管道写
        primes_process(p[0]);
        close(p[0]);
        
        exit(0);
    } else {            // parent
        close(p[0]);    // 只需写，关闭 管道读
        for (i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(i));
        }
        i = -1;
        write(p[1], &i, sizeof(i));		// 写入 -1 表示结束
        close(p[1]);

        wait(0);
        exit(0);
    }
}
