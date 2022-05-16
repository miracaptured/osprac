#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // IPC дескриптор для очереди сообщений
    int msqid;
    pid_t getpid(void);

    // IPC-ключ и название фвйла для генерации
    key_t key;
    char pathname[] = "task03-1.c";

    // длина нужной-полезной части сообщения
    int len, maxlen;

    // структура сообщения
    struct msgbuf {
        long mtype;
        struct {
            pid_t pid;
            float finfo;
        } info;
    } mybuf;

    // структура сообщения сервера
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

    // зааааполняем сообщение данными
    mybuf.mtype = 1;
    mybuf.info.pid = getpid();

    // считываем число
    printf("Enter float number: ");
    scanf("%f", &(mybuf.info.finfo));

    // выясняем длину полезной части
    len = sizeof(mybuf.info);

    // отправляем сообщение в очередь
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    printf("Message from %d was sent.\n", getpid());

    // задаем максимально возможную длину полезной чати сообщения
    maxlen = sizeof(myservbuf.finfo);

    // пробуем прочесть сообщение из очереди
    if ((len = msgrcv(msqid, &myservbuf, maxlen, getpid(), 0)) < 0) {
        printf("Can't receive message from queue\n");
        exit(-1);
    }

    printf("Response:\t%f\n", myservbuf.finfo);

    return 0;
}