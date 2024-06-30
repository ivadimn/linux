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
	pid_t pid = 0;

	if (argc != 2) {
		printf("Usage %s %s\n", argv[0], "<num_process>");
		exit(1);
	}
	
    pid = strtoul(argv[1], NULL, 10);
	
	do{
		printf("%d\n", pid);
		pid = get_ppid(pid);
	} while (pid > 0);
	
	return 0;
}
