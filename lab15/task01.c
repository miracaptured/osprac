#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int received;
int byte;
pid_t sender_pid;

void zero(int signal) {
  byte <<= 1;
  kill(sender_pid, SIGUSR1);
}

void one(int signal) {
  received |= byte;
  byte <<= 1;
  kill(sender_pid, SIGUSR1);
}

int main() {
  pid_t receiver_pid;
  received = 0;
  byte = 1;
  signal(SIGUSR1, zero);
  signal(SIGUSR2, one);

  receiver_pid = getpid();
  printf("Receiver pid: %d\n", receiver_pid);

  printf("Sender pid:\n");
  scanf("%d", &sender_pid);
  kill(sender_pid, SIGUSR1);

  while (1) {
    if(byte == 0) {
      printf("Received value: %d\n", received);
      break;
    }
  }
}
