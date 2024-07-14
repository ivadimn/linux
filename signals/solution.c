#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>


static int usr1_count = 0;
static int usr2_count = 0;

void usrsignal_handler(int signal)
{
    if (signal == SIGUSR1)
        usr1_count++;
    if (signal == SIGUSR2)
        usr2_count++;
}

void sig_term(int signal) {
    if (signal == SIGTERM)
    {
        printf("%d %d\n", usr1_count, usr2_count);
        exit(EXIT_SUCCESS);
    }
    
}

int main(/*int argc, char** argv*/)
{
    void (*funcptr)(int);

    funcptr = signal(SIGUSR1, usrsignal_handler);
    if (funcptr == SIG_ERR)
    {
        printf("Error call signal: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    funcptr = signal(SIGUSR2, usrsignal_handler);
    if (funcptr == SIG_ERR)
    {
        printf("Error call signal: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    funcptr = signal(SIGTERM, sig_term);
    if (funcptr == SIG_ERR)
    {
        printf("Error call signal: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        usleep(500000);
    }

    return 0;
}

