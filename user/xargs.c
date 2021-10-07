#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"
#include "kernel/param.h"
#define MAXCHAR 512

int main (int argc, char *argv[]) {
    char* _argv[MAXARG];
    int i = 0;
    for ( ; i + 1 < argc; ++i){
        _argv[i] = argv[i+1];
    }
    char buf[MAXCHAR];
    int index = 0;
    char c;
    while (read(0, &c, sizeof(c)) == sizeof(c)) {
        if (c == ' ') {
            char temp[index];
            memcpy(temp, buf, index);
            _argv[i++] = temp;
            index = 0;
        }
        else if (c == '\n') {
            char temp[index];
            memcpy(temp, buf, index);
            _argv[i] = temp;
            _argv[i][index] = '\0';
            index = 0;
            i = argc - 1;
            if (fork() == 0) {
                exec(_argv[0], _argv);
            }
            else {
                wait(0);
            }
        }
        else {
            buf[index++] = c;
        }
    }
    exit(0);
}