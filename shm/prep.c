#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>

#define MEM_SIZE 1000
#define SHM_MODE 0600

int main(int argc, char** argv)
{
    key_t source1, source2;
    int shm_id1, shm_id2;
    int *sh1 = NULL;
    int *sh2 = NULL;

    source1 = ftok(argv[0], 99);
    source2 = ftok(argv[0], 199);
    printf("key 1 = %d, key 2 = %d\n", source1, source2);

    shm_id1 = shmget(source1, MEM_SIZE, SHM_MODE | IPC_CREAT) ;
    if (shm_id1 == -1)
    {
        printf("Error with creating shared memory area 1 %d: %s\n", source1, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("id1 = %d\n", shm_id1);
    shm_id2 = shmget(source2, MEM_SIZE, SHM_MODE | IPC_CREAT);
    if (shm_id2 == -1)
    {
        printf("Error with creating shared memory area 2 %d: %s\n", source2, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("id2 = %d\n", shm_id2);

    sh1 = (int*) shmat(shm_id1, sh1, 0);
    if (sh1 == (void*)-1)
    {
        printf("Error with addressing shared memory area: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    sh2 = (int*) shmat(shm_id2, sh2, 0);
    if (sh2 == (void*) -1)
    {
        printf("Error with addressing shared memory area: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("address1 = %p, address2 = %p\n", (void*) sh1, (void*) sh2);
    for (size_t i = 0; i < 10; i++)
    {
        sh1[i] = i * 2;
        sh2[i] = i * 2 + 10;
    }
 
    while (1)
    {
        usleep(500000);
    }

    return 0;
}