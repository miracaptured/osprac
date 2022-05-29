#include <signal.h>
#include <stdio.h>

void signal_handler(int in_signal) {
    // Обработка сигнала в соответствии с его кодом
    // SIGINT - 2
    // SIGQUIT - 3
    switch (in_signal) {
        case 2:
            printf("Pressed: CTRL + C\n");
            printf("Signal: SIGINT %d\n", in_signal);
            break;
        case 3:
            printf("Pressed: CTRL + 4\n");
            printf("Signal: SIGQUIT %d\n", in_signal);
            break;
    }
}

int main() {
    // Обработка SIGINT
    // Теперь Ctrl + C не завершает процесс
    (void)signal(SIGINT, signal_handler);

    // Обработка SIGQUIT
    // Теперь Ctrl + 4 не завершает процесс
    (void)signal(SIGQUIT, signal_handler);

    // Вечный цикл, чтобы процесс не завершился сам
    while(1);
    return 0;
}
