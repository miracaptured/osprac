#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int D(int sem, struct sembuf *buf) {
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;
    return semop(sem, buf, 1);
}

int A(int sem, struct sembuf *buf) {
    buf->sem_op = 1;
    buf->sem_flg = 0;
    buf->sem_num = 0;
    return semop(sem, buf, 1);
}

int main() {
    // указатель на разделяемую память
    int *array;

    // необходимо ли инициализировать элементы массива?
    int new = 1;
    long i;

    int shmid;
    int semid;
    char pathname[] = "task02-1.c";
    

    key_t key;
    struct sembuf mybuf;

    // генерируем IPC-ключ из имени файла
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    // создаем сегмент разделяемой памяти,
    // либо получаем его идентификатор, если он уже существует
    if ((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        // проверяем, в чем проблема и выводим сообщения
        if (errno != EEXIST) {
            printf("Can\'t create shared memory\n");
            exit(-1);
        } else {
            if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0) {
                printf("Can\'t find shared memory\n");
                exit(-1);
            }
            new = 0;
        }
    }

    // отображение разделяемой памяти в адресное пространство текущего процесса
    if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    // пытаемся получить досту по ключу к массиву семафоров
    if ((semid = semget(key, 1, 0666)) < 0) {
        // Если его нет, пытаемся создать
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can\'t create semaphore set\n");
            exit(-1);
        }
        
        // печатаем сообщение
        printf("Success\n");
        
        // увеличиваем счетчик
        A(semid, &mybuf);
    }

    // если нужно инициализировать элементы массива, делаем это
    if (new) {
        array[0] = 0;
        array[1] = 1;
        array[2] = 1;
    } else {
        // уменьшаем счетчик
        D(semid, &mybuf);

        array[0] += 1;
        for (i = 0; i < 2000000000L; i++) ;
        array[2] += 1;

        // увеличиваем счетчик
        A(semid, &mybuf);
    }

    // печатаем сообщение
    printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
           array[0], array[1], array[2]);

    // удаление разделяемой памяти из адресного пространства текущего процесса
    if (shmdt(array) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }

    return 0;
}
