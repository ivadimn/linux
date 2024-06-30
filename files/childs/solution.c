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
#define LOCAL_LEN 2048
#define BUFF_LEN 4096

static int count_proc = 0;


int get_childs(char *dir) {
	char name[PATH_LEN];
	char buffer[BUFF_LEN];
	char* saveptr;
	int read_count = 0, count = 0;
	int fd;
	snprintf(name, PATH_LEN - 1, "%s/children", dir);

	fd = open(name, O_RDONLY);
	if (fd == -1) {
		return 0;
	}
	read_count  = read(fd, buffer, BUFF_LEN);
    if (read_count <= 0) {
		close(fd);
		return 0;
    }
    close(fd);
	
	char *data = strtok_r(buffer, " ", &saveptr);
	while(data) {
		count++;
		data = strtok_r(NULL, " ", &saveptr);
	}
	return count;
}

int get_count(char* dir) {
	int count = 0;
	struct dirent *dp;
    DIR *dfd;

	if((dfd = opendir(dir))==NULL) {
        return 0;
    }
  
    while((dp=readdir(dfd)) != NULL){
        
        if(strcmp(dp->d_name,".") == 0 || 
            strcmp(dp->d_name,"..") == 0 )
            continue;
		count++;
    }
    closedir(dfd);
	return count;
}

void view_dir(char* dir) {
	char name[PATH_LEN];
	char local_name[LOCAL_LEN];
    struct dirent *dp;
    DIR *dfd;
	snprintf(name, PATH_LEN - 1, "%s/task", dir);
	
	if (get_count(name) == 0)
		return;
	
	if((dfd = opendir(name)) == NULL) {
        return;
    }
	while((dp=readdir(dfd)) != NULL){
        
        if(strcmp(dp->d_name,".") == 0 || 
            strcmp(dp->d_name,"..") == 0 )
            continue;
		sprintf(local_name,"%s/%s", name, dp->d_name);	
		int ch = get_childs(local_name);	
		count_proc += ch;
		view_dir(local_name);
		count_proc++;
    }

}

int main(int argc, char** argv) {

	char name[PATH_LEN];
    
    if (argc != 2) {
		printf("Usage %s %s\n", argv[0], "<num_process>");
		exit(1);
	}
	snprintf(name, PATH_LEN - 1, "/proc/%s", argv[1]);
	view_dir(name);
	printf("%d\n", count_proc - 1);
	return 0;
}
