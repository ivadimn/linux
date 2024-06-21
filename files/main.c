#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {

    char buf[100];
    size_t sz = 0;

    if(argc != 2) {
        printf("\nPlease call:\n\t%s filename\n", argv[1]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    perror("fd");
    printf("fd = %d\n", fd);
    sz = read(fd, buf, 100);
    printf("sz = %ld\n", sz);

    return 0;
}

