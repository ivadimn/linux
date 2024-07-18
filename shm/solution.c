#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define MEM_SIZE 1000
#define SHM_MODE 0600
#define SHM_MODE_READ 0400

int* get_shmem(key_t key, size_t size, int flag) 
{
    int *buf = NULL;
    int id;
    id = shmget(key, size, flag);
    if (id == -1)
    {
        printf("Error creating shared memory with key %x: %s\n", key, strerror(errno));
        exit(EXIT_FAILURE);
    }
    buf = (int*) shmat(id, buf, 0);
    if (buf == (void*)-1)
    {
        printf("Error with addressing shared memory with key %x: %s\n", key, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return buf;
}


int main(int argc, char** argv)
{
    key_t source1, source2, dest_key;
    int *sh1 = NULL;
    int *sh2 = NULL;
    int *dest = NULL; 

    if(argc != 3) {
        printf("\nPlease call:\n\t%s key1 key2\n", argv[0]);
        return 1;
    }
    source1 = atoi(argv[1]);
    source2 = atoi(argv[2]);
    
    dest_key = ftok(argv[0], 99);

    sh1 = get_shmem(source1, 0, SHM_MODE_READ);
    sh2 = get_shmem(source2, 0, SHM_MODE_READ);
    dest = get_shmem(dest_key, MEM_SIZE, SHM_MODE | IPC_CREAT);

    for (size_t i = 0; i < 100; i++)
    {
       dest[i] = sh1[i] + sh2[i];
    }
   
    printf("%d\n", dest_key);

    return 0;
}

