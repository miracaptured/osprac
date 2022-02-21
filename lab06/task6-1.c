
#include <sys/types.h> 
#include <unistd.h> 
#include <stdio.h> 
  
int main() { 
    // Дескриптор для pipe 
    int fd[2]; 
 
    // Размер записанного сообщения 
    int size; 
    // Байт который мы добавляем
    char *b =  malloc(1 * sizeof(char)); 
   
    int res; 
 
    // Создаем pipe и проверяем, что все хорошо 
    if (pipe(fd) < 0) { 
        printf("Can\'t open pipe\n"); 
        exit(-1); 
    } 
 
     
    // Записываем сообщение, прибавляем по байту на каждой итерации 
    // Предел достигнут, когда байты уже не пишутся
    do { 
        size = write(fd[1], b, 1); 
        res++; 
        printf("%d\n", res); 
    } while (size == 1); 
 
   
   // На локальной машине код выдает последний резлуьтат 65536 байт 
  
    res--; 
 
    // Выводим итог 
    printf("Pipe size: [%d bytes]\n", res); 
 
    return 0; 
}
