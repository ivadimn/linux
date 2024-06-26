#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define PATH_LEN 1024
#define BUFF_LEN 4096


pid_t get_ppid(pid_t pid) {
	char *saveptr;
	char proc_status[PATH_LEN];
	char buffer[BUFF_LEN];
	size_t count = 0;
	pid_t ppid;
	
	
	snprintf(proc_status, PATH_LEN - 1, "/proc/%d/stat", pid); 
	int fd = open(proc_status, O_RDONLY);
	if (fd == -1) {
		printf("Ошибка открытия файла %s : %s\n", proc_status, strerror(errno));
		exit(1);
	}
	count  = read(fd, buffer, BUFF_LEN);
    if (count < 0) {
    	printf("Ошибка чтения файла %s : %s\n", proc_status, strerror(errno));
		close(fd);
		exit(1);
    }
    close(fd);
	int index = 0;

	char *data = strtok_r(buffer, " ", &saveptr);
	while(data) {
		if(index == 3) {
			ppid = atoi(data);
			break;
		}
		index++;
		data = strtok_r(NULL, " ", &saveptr);
	}
	return ppid;
}

int main(int argc, char** argv) {
    char cmdline[64];
    char line[32];
    snprintf(cmdline, 63, "pidof %s",argv[0]);
    FILE *cmd = popen(cmdline, "r");
    fgets(line, 32, cmd);
    pid_t pid = strtoul(line, NULL, 10);
	pclose(cmd);
	printf("%d\n", get_ppid(pid));
}
