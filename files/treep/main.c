#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
        
int main(int argc, char** argv) {
    char cmdline[64];
    char line[32];
    snprintf(cmdline, 63, "pidof %s",argv[0]);
    FILE *cmd = popen(cmdline, "r");
    fgets(line, 32, cmd);
    pid_t pid = strtoul(line, NULL, 10);
    pclose(cmd);
    printf("pid = %d\n", pid);
    pid = getpid();
    printf("pid = %d\n", pid);
}
