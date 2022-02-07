#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main() {
    pid_t thisProcessID = getpid();
    pid_t parentProcessID = getppid();
    pid_t forkedProcessID = fork();

    printf("PID = %d\n", thisProcessID);
    printf("PPID = %d\n", parentProcessID);

    if (forkedProcessID < 0) {
        printf("we have problems with forking process... (%d)\n\n", forkedProcessID);
        exit(-1);
    }
    else if (forkedProcessID == 0) {
        printf("This is from child process. (%d)\n", forkedProcessID);
        printf("let's start task03-1 \n\n");
        execl("./task03-1.out", "daisuki,", "dayo!", NULL);
        exit(0);
    }
    else
        printf("This is from parent process. (%d)\n\n", forkedProcessID);

    return 0;
}