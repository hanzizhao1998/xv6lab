#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if (argc != 1) {
        fprintf(2, "Usage : pingpong ...");
        exit(-1);
    }
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    char buffer[4];
    int childpid = fork();
    if (childpid != 0) {
        write(p1[1], "pong", 4);
        read(p2[0], &buffer, 4);
        wait(&childpid);
        fprintf(1, "%d: received %s\n", getpid(), buffer);
    }
    else {
        read(p1[0], &buffer, 4);
        write(p2[1], "ping", 4);
        fprintf(1, "%d: received %s\n", getpid(), buffer);
    }
    exit(0);
}