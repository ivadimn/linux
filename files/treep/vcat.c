#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


#define BUFF_LEN 4096

int64_t get_file_size(int fd) {
	int64_t fsize = 0;
	struct stat fileStatbuff;
	if ((fstat(fd, & fileStatbuff) != 0) || (!S_ISREG(fileStatbuff.st_mode))) {
		fsize = -1;
	}
	else{
		fsize = fileStatbuff.st_size;
        
	}
	return fsize;
}

void print_file(char *name) {
  FILE *f = fopen(name, "rt");
    
  if (f != NULL) {
  int c = fgetc(f);
  while (c != EOF) {
    putc(c, stdout);
    c = fgetc(f);
  }
  fclose(f);
  }
}

int main(int argc, char** argv)  {

    char buffer[BUFF_LEN];
    size_t count = 0;

    if (argc != 2)   {
        printf("Ошибка, необходимо указать имя файла.");
		exit(1);
    }

    print_file(argv[1]);
    printf("\n");
    
	int fd = open(argv[1], O_RDONLY, S_IRUSR);
	if (fd == -1) {
		printf("Ошибка открытия файла %s : %s\n", argv[1], strerror(errno));
		exit(1);
	}
	//int64_t fsize = get_file_size(fd);
	//printf("size: %ld\n",fsize);
	do {
        count  = read(fd, buffer, BUFF_LEN);
        if (count < 0) {
            printf("Ошибка чтения файла %s : %s\n", argv[1], strerror(errno));
		    close(fd);
		    exit(1);
        }
        printf("%s", buffer);
    } while (count == BUFF_LEN);
    
    printf("\n");
	
}
