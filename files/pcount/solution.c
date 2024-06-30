#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>

#define PATH_LEN 1024
#define BUFF_LEN 4096

const char* root_dir = "/proc";

int is_dir(char* name) {
    struct stat stat_buff;
    int result = stat(name, &stat_buff);
    if (result == -1) {
        printf("Ошибка при получении информации о файле: %s", name);
        return  0;
    }
    return S_ISDIR(stat_buff.st_mode) ? 1 : 0; 
}


int is_pname(char* pid, const char* pname) {
	char *saveptr;
	char proc_status[PATH_LEN];
	char buffer[BUFF_LEN];
	size_t count = 0;
		
	snprintf(proc_status, PATH_LEN - 1, "/proc/%s/stat", pid); 
	int fd = open(proc_status, O_RDONLY);
	if (fd == -1) {
		return 0;
	}
	count  = read(fd, buffer, BUFF_LEN);
    if (count < 0) {
		close(fd);
		return 0;
    }
    close(fd);
	return (strstr(buffer, pname) != NULL) ? 1 : 0;
}

int main(int argc, char** argv) {

	char name[PATH_LEN];
    struct dirent *dp;
    DIR *dfd;
    int count = 0;

    if((dfd = opendir(root_dir))==NULL) {
        printf("Ошибка открытия каталога: %s", root_dir);
        return 1;
    }
  
    while((dp=readdir(dfd)) != NULL){
        
        if(strcmp(dp->d_name,".") == 0 || 
            strcmp(dp->d_name,"..") == 0 )
            continue;
		sprintf(name,"%s/%s",root_dir, dp->d_name);
		if (is_dir(name)) {
			count += is_pname(dp->d_name, "genenv");	
		}
    }
    closedir(dfd);
	printf("%d\n", count);
	return 0;
}
