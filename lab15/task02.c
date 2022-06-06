#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int next = 1;

void handle(int sig) {
    next = 0;
}

int main() {
  pid_t sender_pid, to_pid;
  int input;
  (void)signal(SIGUSR1, handle);

  sender_pid = getpid();
  printf("Sender pid: %d\n", sender_pid);

  printf("Input pid:\n");
  scanf("%d", &to_pid);

  printf("Input value:\n");
  scanf("%d", &input);

  int byte = 1;
  while (next);
  while (byte != 0) {
    if ((input & byte) == 0) {
      kill(to_pid, SIGUSR1);
    } else {
      kill(to_pid, SIGUSR2);
    }
    byte = byte << 1;
    next = 1;
    while(next);
  }
}
