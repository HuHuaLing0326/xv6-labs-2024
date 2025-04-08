#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    char buf[512];
    char* s = buf;
    char* nargv[MAXARG];
    for(int i = 0;i < argc-1;i++){
        char* na = malloc(sizeof(**argv));
        strcpy(na, argv[i+1]);
        nargv[i] = na;
    }
    // 从标准输入读取参数，读到一个换行就执行一条命令
    memset(buf,0,512);
    while(read(0,s,1)){
        if(*s == '\n'){
            *s = 0;
            // 创建子进程，将读取的一行添加为命令的最后一个参数，执行命令
            if(fork() == 0){
                char* na = malloc(sizeof(**argv));
                strcpy(na,(const char*)buf);
                nargv[argc-1] = na;
                nargv[argc] = 0;
                exec(nargv[0],nargv);
            }
            wait((int *)0);
            memset(buf,0,512);
            s = buf-1;
        }
        s++;
    }
    exit(0);
    
}