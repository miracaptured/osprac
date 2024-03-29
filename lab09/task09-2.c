#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
  int   semid;
  char pathname[]="task09-1.c";
    
  key_t key;
  struct sembuf mybuf;

  // генерируем IPC-ключ из имени первого файла
  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

    // пытаемся получить доступ по ключу к массиву семафоров,
    // если его не существует, создаем массив из одного семафора
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }
  
  // определяем структуру
  mybuf.sem_num = 0;
  mybuf.sem_op  = 1;
  mybuf.sem_flg = 0;

  // увеличиваем счетчик
  if (semop(semid, &mybuf, 1) < 0) {
    printf("Can\'t add 1 to semaphore\n");
    exit(-1);
  }

  // печатаем сообщение
  printf("The condition is present\n");

  return 0;
}