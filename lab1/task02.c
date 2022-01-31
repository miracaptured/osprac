
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t thisProcessID = getpid();
    pid_t parentProcessID = getppid();

    printf("PID = %d\n", thisProcessID);
    printf("PPID = %d\n", parentProcessID);
    return 0;
}
