#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char** argv)
{
    key_t source1, source2, dest;
    
    if (argc != 3)
    {
        printf("Usage %s key_memory1 key_memory2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    source1 = atol(argv[1]);
    source1 = atol(argv[2]);
    
    while (1)
    {
        usleep(500000);
    }

    return 0;
}

