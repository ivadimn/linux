
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void sig_urg(int signal) 
{
    if (signal == SIGURG)
    {
        exit(EXIT_SUCCESS);
    }
}

int main(/*int argc, char** argv*/) {
    
    void (*funcptr)(int);

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
    funcptr = signal(SIGURG, sig_urg);
    if (funcptr == SIG_ERR)
    {
        fprintf(stdout, "Ошибка выполнения функции signal: %s\n", strerror(errno));
    }
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    while (1)
    {
        sleep(1);
    }
}