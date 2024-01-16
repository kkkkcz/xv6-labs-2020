#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h" // 必须以这个顺序 include，由于三个头文件有依赖关系

void sieve(int pleft[2]) {
    int p;
    read(pleft[0], &p, sizeof(p));  // 读取第一个数，素数
    
    if (-1 == p) {  // 说明所有数字都处理完毕了
        exit(0);
    }
    printf("prime %d\n", p);

    int pright[2];  
    pipe(pright);   // 创建用于输出到下一 stage 的进程的输出管道 pright

    if (fork() == 0) {  // 子进程
        close(pleft[0]);    // 子进程用不到pleft管道
        // close(pleft[1]); // 这里不能关闭pleft的写入端，因为后续用到的递归使得子进程作为父进程，而在关闭写入端之后是无法重新打开的
        close(pright[1]);   // 子进程只需要从pright读取数字
        sieve(pright);
    }
    else {  // 当前进程
        close(pright[0]);   
        // close(pleft[1]);
        int buf;
        while (read(pleft[0], &buf, sizeof(buf)) && buf != -1) {
            if (buf % p != 0)
                write(pright[1], &buf, sizeof(buf));
        }
        buf = -1;
        write(pright[1], &buf, sizeof(buf));
        wait(0);
        exit(0);
    }
}

int main() {
    int input_pipe[2];  
    pipe(input_pipe);   // 创建用于输入数字的管道
    if (fork() == 0) {  // 子进程
        close(input_pipe[1]);   // 子进程只需要从输入管道读取数字，关闭写端减少对文件描述符的占用
        sieve(input_pipe);
    }
    else {  // 父进程
        close(input_pipe[0]);   // 关闭读端
        int i;
        for (i = 2; i <= 35; i++) {
            write(input_pipe[1], &i, sizeof(i));
        }
        // 写入完成后，输入-1作为结束符号
        i = -1;
        write(input_pipe[1], &i, sizeof(i));
    }
    wait(0);
    exit(0);
}