#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


#define RD 0 //pipe的read端
#define WR 1 //pipe的write端

int main() {
    int s2p[2]; // 子进程->父进程
    int p2s[2]; // 父进程->子进程
    // char *argv[2];
    // argv[0] = "wc";
    // argv[1] = 0;
    pipe(p2s);
    pipe(s2p);
    // int pid = fork();
    if (fork() == 0)
    {
        close(s2p[RD]); // 关闭子进程->父进程管道的读取端
        close(p2s[WR]);
        char buffer[5];
        read(p2s[RD], &buffer, 4);
        printf("%d: received %s\n", getpid(), buffer);
        close(p2s[RD]);    // 关闭父进程->子进程读端

        write(s2p[WR], "pong", 4);
        close(s2p[WR]);
        exit(0);
    }
    else
    {
        close(p2s[RD]);
        write(p2s[WR], "ping", 4);
        close(p2s[WR]);
        wait(0);
        char buffer[5];
        read(s2p[RD], &buffer, 4);
        printf("%d: received %s\n", getpid(), buffer);
        close(s2p[RD]);
        exit(0);
    }
    return 0;
}