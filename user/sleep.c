#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if (2 != argc) {    // 缺少参数或参数过多
        fprintf(2, "Missing argument, usage: sleep <ticks>\n");
        exit(1);
    } 
    int ticks = atoi(argv[1]);  // 将字符串转为整数
    sleep(ticks);    
    exit(0);
}