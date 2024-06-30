
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd;
        
    switch (fork())         //превращение в фоновый процесс 
    {
        case -1: 
            printf("Ошибка вызова функции fork\n");
            _exit(EXIT_FAILURE);
        case 0:  break;                 //потомок проходит этот этап
        default: _exit(EXIT_SUCCESS);   //родитель завершается
    }

    if (chdir("/") < 0)          //переходим в корневой каталог
        printf("Невозможно сделать текущим рабочим каталогом\n");

    if (setsid() == -1) {                 //процесс становится лидером новой сессии
        printf("Не удалось стать лидером новой сессии\n");
        _exit(EXIT_FAILURE);
    }

    pid_t pid = getpid();
    fprintf(stdout, "%d\n", pid);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    sleep(60);
}