#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_PRIME 31
void doPrime(int [], int []);

int main (int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(2, "Usage : primes ...\n");
        exit(-1);
    }
    int leftPipe[2];
    int rightPipe[2];
    pipe(leftPipe);
    pipe(rightPipe);
    //child process
    if(fork() == 0) 
        doPrime(leftPipe, rightPipe);
    //parent process
    else{
        close(leftPipe[0]);
        close(rightPipe[0]);
        close(rightPipe[1]);
        for (int i = 2; i <= 1000 ; ++i) {
            write(leftPipe[1], &i, sizeof(i));
        }
        close(leftPipe[1]);
    }
    wait(0);
    exit(0);
}

void doPrime(int leftPipe[], int rightPipe[]) {
    int prime;
    if(read(leftPipe[0], &prime, sizeof(prime)) > 0 )
        printf("prime %d\n", prime);
    if (prime == MAX_PRIME) {
        close(leftPipe[0]);
        close(leftPipe[1]);
        close(rightPipe[0]);
        close(rightPipe[1]);
        return;
    }
    //child process
    if (fork() == 0) {
        int p[2];
        close(leftPipe[0]);
        close(leftPipe[1]);
        pipe(p);
        doPrime(rightPipe, p);
    }
    //parent process
    else {
        close(leftPipe[1]);
        close(rightPipe[0]);
        int n;
        while(read(leftPipe[0], &n, sizeof(n)) != 0){
            if((n % prime) != 0)
                write(rightPipe[1], &n, sizeof(n));
        }
        close(leftPipe[0]);
        close(rightPipe[1]);
        wait(0);
    }
}


