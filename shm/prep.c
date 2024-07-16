#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define MEM_SIZE 1000

int main(int argc, char** argv)
{
    key_t source1, source2;
    int shm_id1, shm_id2;
    int *sh1 = NULL;
    int *sh2 = NULL;

    source1 = ftok(argv[0], 99);
    source1 = ftok(argv[0], 199);

    shm_id1 = shmget(source1, MEM_SIZE, IPC_CREAT);
    if (shm_id1 == -1)
    {
        printf("Error with creating  shared memory area\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    shm_id2 = shmget(source2, MEM_SIZE, IPC_CREAT);
    if (shm_id2 == -1)
    {
        printf("Error with creating  shared memory area\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    sh1 = (int*) shmat(shm_id1, NULL, SHM_RND)
    




    while (1)
    {
        usleep(500000);
    }

    return 0;
}