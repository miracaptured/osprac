#include <signal.h>

int main() {
    // Игнорирование SIGINT
    // Теперь Ctrl + C не завершает процесс
    (void)signal(SIGINT, SIG_IGN);

    // Игнорирование SIGQUIT
    // Теперь Ctrl + 4 не завершает процесс
    (void)signal(SIGQUIT, SIG_IGN);

    // Вечный цикл, чтобы процесс не завершился сам
    while(1);
    return 0;
}
