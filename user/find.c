// find.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *target) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
    case T_FILE: /* (my note) Address file type */
        // 如果文件名结尾匹配 `/target`，则视为匹配
        if (strcmp(path + strlen(path) - strlen(target), target) == 0)
            printf("%s\n", path);
        break;

    case T_DIR: /* (my note) Address dir type */
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);  // 将原始路径path(目录项)拷贝到缓冲区buf
        p = buf + strlen(buf);  // 此时p指向缓冲区的末尾
        *p++ = '/'; // 在路径末尾加上 / 为后续文件名拼接做准备
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);    // 将目录项的名称拷贝到缓冲区中
            p[DIRSIZ] = 0;  // 字符串结束符
            if (stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            if (strcmp(buf + strlen(buf) - 2, "/.") != 0 && strcmp(buf + strlen(buf) - 3, "/..") != 0) {
                find(buf, target);
            }
        }
        break;
    }
    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc < 3) 
        exit(0);
    char target[512];
    target[0] = '/';    // 为查找的文件名添加 / 在开头
    strcpy(target + 1, argv[2]);    // 注意这里是argv[2],argv[1]是要查找的路径
    find(argv[1], target);
    exit(0);
}