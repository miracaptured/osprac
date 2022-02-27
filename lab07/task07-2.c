#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <errno.h> 
#include <stdlib.h> 
 
int main() { 
    int shmid; 
    int new = 1; 
    char pathname[] = "task07-1.c"; 
    key_t key; 
    char *text; 
    int *size; 
    int fsize; 
    FILE *file; 
 
    // Открываем файл с исходным кодом программы на чтение 
    file = fopen(pathname, "r"); 
 
    // Получаем размер файла 
    fseek(file, 0L, SEEK_END); 
    fsize = ftell(file); 
 
    // Перемещаем указатель на начало файлового потока 
    rewind(file); 
    fclose(file); 
 
    // Генерация IPC-ключа 
    if ((key = ftok(pathname, 0)) < 0) { 
        printf("Can\'t generate key\n"); 
        exit(-1); 
    } 
 
    //  создаем сегмент разделяемой памяти, 
    // либо получение его идентификатора, если он уже существует 
    if ((shmid = shmget(key, sizeof(int), 0666)) < 0) { 
        printf("Can\'t find shared memory\n"); 
        exit(-1); 
    } 
 
    // Отображение разделяемой памяти в адресное пространство текущего процесса 
    if ((size = (int *) shmat(shmid, NULL, 0)) == (int *) (-1)) { 
        printf("Can't attach shared memory\n"); 
        exit(-1); 
    } 
    text = (char *) (size + 1); 
    // Печатаем данные из разделяемой памяти в консоль 
 
    for (int i = 0; i < *size; i++) putchar(text[i]); 
    // Удаление разделяемой памяти из адресного пространства текущего процесса 
    // и завершение работы 
    if (shmdt(size) < 0) { 
        printf("Can't detach shared memory\n"); 
        exit(-1); 
    } 
    // Удаление сегмента разделяемой памяти из системы 
    if (shmctl(shmid, IPC_RMID, NULL) < 0) { 
        printf("Can't delete shared memory\n"); 
        exit(-1); 
    } 
    return 0; 
}
