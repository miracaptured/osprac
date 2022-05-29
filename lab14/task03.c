#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

void signal_handler() {
    int status;
    pid_t pid;

    pid = waitpid(-1, &status, WNOHANG);
    while (pid) {
        if (pid < 0) {
            if (errno == 10) {
                printf("All child processes finished\n");
            } else {
                printf("ERROR ON WAITPID: errno = %d\n", errno);
            }
            return;
        } else {
            if ((status & 0xff) == 0) {
                printf("Process %d exited with status %d\n", pid, status >> 8);
            } else if ((status & 0xff00) == 0) {
                printf("Process %d killed by signal %d %s\n", pid, status & 0x7f,
                       (status & 0x80) ? "with core file" : "without core file");
            }
            pid = waitpid(-1, &status, WNOHANG);
        }
    }
}

int main() {
    pid_t pid;
    int i, j;

    (void)signal(SIGCHLD, signal_handler);

    for (i = 0; i < 10; ++i) {
        if ((pid = fork()) < 0) {
            printf("Cannot fork child 1\n");
            exit(1);
        } else if (pid == 0) {
            // Child
            for (j = 1; j < 10000000; ++j);
            exit(200 + i);
        }
    }

    while (1);
    return 0;
}
