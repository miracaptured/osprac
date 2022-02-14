#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int fd;
    int size = 14;
    char text[size];
    char *filename = "myfile";

    (void)umask(0);

    // Открываем файлик, который создает запуск программы из примера
    if ((fd = open(filename, O_RDONLY)) < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }

    // Считываем в переменную
    if (read(fd, text, size) != size) {
        printf("Can\'t read file\n");
        exit(-1);
    }

    // Выводим
    printf("%d bytes from file %s:\n%s\n", size, filename, text);

    // Закрываем поток и проверяем корректность
    if (close(fd) < 0) {
        printf("Can\'t close file\n");
        exit(-1);
    }

    return 0;
}
