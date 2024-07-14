
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_LEN 255

long get_number(char* str)
{
    char *start = str;
    char *end = NULL;
    long number = 0;
    while(1)
    {
        number += strtol(start, &end, 10);
        if (start == end)
            break;
        start = end;
    }
    return number;
}

int main(/*int argc, char** argv*/)
{
    char buf[BUF_LEN + 1];
    int fds[2];
    int bytes = 0;
    long number = 0;

    fds[0] = open("./in1", O_RDONLY);
    fds[1] = open("./in2", O_RDONLY);
    

    if(!(fds[0] && fds[1])) {
        printf("Error with opening files\n");
        return 1;
    }
    
    fd_set read_set;
    fd_set watchset;
    FD_ZERO(&watchset);
    FD_SET(fds[0], &watchset);
    FD_SET(fds[1], &watchset);
    while (FD_ISSET(fds[0], &watchset) || FD_ISSET(fds[1], &watchset))
    {
        read_set = watchset;   
        if(select(fds[1] + 1, &read_set, NULL, NULL, NULL) < 0) {
            printf("Error call select\n");
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < 2; i++)
        {
            if (FD_ISSET(fds[i], &read_set))
            {
                bytes = read(fds[i], buf, BUF_LEN);
                if (bytes < 0)
                {
                    printf("Error read descriptor %d\n", fds[i]);
                    exit(EXIT_FAILURE);
                }
                else if(bytes == 0)
                {
                    close(fds[i]);
                    FD_CLR(fds[i], &watchset);
                }
                else 
                {
                    buf[bytes] = 0;
                    number += get_number(buf);
                }
            }
        }
    }
    printf("%ld\n", number);
    return 0;
}

