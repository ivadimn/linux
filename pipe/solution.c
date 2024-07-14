
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>

#define BUFF_LEN 4096
#define COMMAND_LEN 128

int zero_count(char* buff, size_t buff_size) {
    int count = 0;
    for (size_t i = 0; i < buff_size; i++)
    {
        if (buff[i] == '0')
            count++;
    }
    return count;
}


int main(int argc, char** argv) {

    char buf[BUFF_LEN];
    char command[COMMAND_LEN];
    size_t sz = 0;
    int count = 0;

    if(argc != 3) {
        printf("\nPlease call:\n\t%s command param\n", argv[0]);
        return 1;
    }
    snprintf(command, COMMAND_LEN - 1, "%s %s", argv[1], argv[2]);

    FILE *f = popen(command, "r");
    if (!f)
    {
        printf("Ошибка получения потока команыд %s: %s\n", command, strerror(errno));
    }
    do
    {
        sz = fread(buf, 1, BUFF_LEN, f);
        count += zero_count(buf, sz); 

    } while (sz == BUFF_LEN);
    pclose(f);
    printf("%d\n", count);
    return 0;
}

