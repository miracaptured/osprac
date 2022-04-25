#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void) {
    // IPC descriptor для очереди (сообщений)
    int msqid;

    // IPC-ключ и название фвйла для генерации
    key_t key;
    char pathname[] = "task11-01-1.c";

    // структура сообщения
    struct mymsgbuf {
        long mtype;
        struct {
            short sinfo;
            float finfo;
        } info;
    } mybuf;

    // длина полезной части сообщения
    int len;

    // генерируем IPC-ключ
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    // пробуем получить доступ к очереди
    // или создаем ее, если она не существует
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    /* отправляем сообщение */

    for (int i = 0; i < 5; i++) {
        // заполняем его данными
        mybuf.mtype = 1;
        mybuf.info.sinfo = i;
        mybuf.info.finfo = i * 1000;

        // выясняем длину полезной части
        len = sizeof(mybuf.info);

        // отправляем сообщение в очередь
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }

        printf("Message [%d] was sent with info = [%d ; %f]\n", i, mybuf.info.sinfo, mybuf.info.finfo);
    }

    /* отправка последнего сообщения */

    mybuf.mtype = LAST_MESSAGE;
    len = 0;

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    return 0;
}
