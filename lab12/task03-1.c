#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
    // IPC дескриптор для очереди сообщений
    int msqid;

    // IPC-ключ и название файлика для генерации
    key_t key;
    char pathname[] = "task03-1.c";

    // длина полезной части сообщения
    int len, maxlen;

    // структура сообщения
    struct msgbuf {
        long mtype;
        struct {
            pid_t pid;
            float finfo;
        } info;
    } mybuf;

    // cтруктура сообщения сервера
    struct servmsgbuf {
        long mtype;
        float finfo;
    } myservbuf;

    // генерируем IPC-ключ
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    // пробуем получить доступ к очереди сообщений
    // или создать ее, если она не существует
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    // читаем сообщения в бесконечном цикле
    // в порядке FIFO (First In - First Out, как в обычной очереди)
    while (1) {
        // задаем максимально возможную длину полезной чати сообщения
        maxlen = sizeof(mybuf.info);

        // пытаемся прочесть сообщение из очереди
        if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, 1, 0)) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }

        printf("Request from client  %d\n", mybuf.info.pid);

        // заполняем данными сообщение
        myservbuf.mtype = mybuf.info.pid;

        // считываем результат
        myservbuf.finfo = mybuf.info.finfo * mybuf.info.finfo;

        // определяем длину нужной-полезной части
        len = sizeof(myservbuf.finfo);

        // отправляем сообщение в очередь
        if (msgsnd(msqid, (struct servmsggbuf *)&myservbuf, len, 0) < 0) {
            printf(" Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
            exit(-1);
        }

        printf("\nResponded\n");
    }

    return 0;
}