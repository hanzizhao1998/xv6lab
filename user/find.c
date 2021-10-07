#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"
#include "kernel/fs.h"

void find (char*, char*);
char* fmtname(char*);
int main (int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(2, "Usage : find <path> <filename> ...\n");
    }
    int length = strlen(argv[2]);
    if(length >= DIRSIZ) {
        fprintf(2, "file name too long...\n");
        exit(-1);
    }
    char file[DIRSIZ+1];
    memmove(file, argv[2], length);
    memset(file + length, ' ', DIRSIZ - length);
    find(argv[1], file);
    exit(0);
}

void find (char* path, char* file) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
    case T_FILE:
        if (strcmp(file, fmtname(path)) == 0)
            printf("%s\n", path);
        break;

    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        printf("%s\n", buf);
        p = buf + strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0){
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            if (strcmp(fmtname(buf), ".             ") == 0 ||
                strcmp(fmtname(buf), "..            ") == 0 )
                continue;
            find(buf, file);
        }
        break;
    }
    close(fd);
}


char* fmtname(char *path) {
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}