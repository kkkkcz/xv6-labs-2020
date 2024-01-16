#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h" // 必须以这个顺序 include，由于三个头文件有依赖关系

int main() {
    int p[2];
    char *argv[2];
    argv[0] = "wc";
    argv[1] = 0;
    pipe(p);    // p[0] 是管道的读取端，p[1] 是写入端
    if (fork() == 0)
    {
        close(0);   // 关闭标准输入
        dup(p[0]);  // 复制管道的读取端到标准输入的文件描述符0
        close(p[0]);
        close(p[1]);
        exec("wc", argv);   // wc: word count,统计文件中的行数、词数、字符数
    }
    else
    {
        close(p[0]);
        write(p[1], "this is a test\n", 18);
        close(p[1]);
    }
    exit(0);
}