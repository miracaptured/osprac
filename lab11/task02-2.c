#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void) {
    // IPC descriptor для очереди сообщений
    int msqid;

    // IPC-ключ и название фвйла для генерации
    key_t key;
    char pathname[] = "task02-1.c";

    // структура сообщения
    struct mymsgbuf {
        long mtype;
        struct {
            char text[23];
            int num;
        } info;
    } mybuf;

    // длина и максимальная длина информативной части сообщения
    int len, maxlen;

    // генерируем IPC-ключ
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    // пытаемся получить доступ к очереди сообщений
    // или создать ее, если она не существует
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }


    /* чтение сообщений */
    printf("Script \'task02-2.c\' started receiving messages\n");

    // в порядке FIFO (First In - First Out, как в обычной очереди)
    for (int i = 0; i < 5; i++) {
        // Задаем максимально возможную длину полезной чати сообщения
        maxlen = sizeof(mybuf.info);

        // пытаемся прочесть сообщение из очереди
        if ((len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0)) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        // печатаем сообщение
        printf("> message type = %ld, info = [%d ; \'%s\']\n", mybuf.mtype, mybuf.info.num, mybuf.info.text);
    }

    printf("Script \'task02-2.c\' finished sending messages\n");
    printf("---\n");

    /* отправка сообщения */
    printf("Script \'task02-2.c\' start sending messages\n");

    for (int i = 0; i < 5; i++) {
        // заполняем сообщение данными
        mybuf.mtype = 2;
        mybuf.info.num = i;
        strcpy(mybuf.info.text, "Message from script 2.");

        // определяем длину полезной части
        len = sizeof(mybuf.info);

        // отправляем сообщение в очередь
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }

        printf("Message [%d] was sent with info = [%d ; \'%s\']\n", i, mybuf.info.num, mybuf.info.text);
    }
    printf("Script \'task02-1.c\' finished sending messages\n");

    return 0;
}
